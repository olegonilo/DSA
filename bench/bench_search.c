/* bench_search — час одного пошуку (нс) залежно від n, включно з n, що не влазять у кеш.
 * Порівнюємо:
 *   linear        — O(n), але послідовний доступ і SIMD-векторизація;
 *   binary        — класичний O(log n) з розгалуженнями (mispredict ~50% на кожному кроці);
 *   branchless    — той самий O(log n), але без розгалужень (csel/cmov);
 *   eytzinger     — "покращення структури даних": масив у порядку BFS-обходу дерева + prefetch.
 *                   Та сама асимптотика, але кращі кеш-промахи — див. Khuong & Morin, 2017;
 *   interpolation — O(log log n) на рівномірних даних.
 * Вихід: results/search_time.csv */
#include "bench_util.h"
#include "dsa_search.h"

#define Q (1 << 20)

/* Eytzinger: b[1..n] — неявне дерево (діти k: 2k, 2k+1), заповнене in-order-обходом із a[]. */
static size_t eytz_build(const int *a, int *b, size_t i, size_t k, size_t n) {
    if (k <= n) {
        i = eytz_build(a, b, i, 2 * k, n);
        b[k] = a[i++];
        i = eytz_build(a, b, i, 2 * k + 1, n);
    }
    return i;
}

static size_t eytz_search(const int *b, size_t n, int key) {
    size_t k = 1;
    while (k <= n) {
        /* k*16: префетч на 4 рівні вперед (16 int = 64 Б — кеш-лінія x86; на Apple M4 hw.cachelinesize = 128). Індекс обмежуємо n:
         * (1) арифметика вказівника за межі масиву — UB (C11 6.5.6p8);
         * (2) префетч на ще не відображені сторінки спричиняє дорогі page walk-и (виміряно: 38 нс
         *     замість ~6 нс при n=1024 у першій версії цього бенчмарку). */
        size_t pf = k * 16;
        pf = pf <= n ? pf : n;
        __builtin_prefetch(b + pf);
        k = 2 * k + (size_t)(b[k] < key);
    }
    k >>= __builtin_ffsll((long long)~k); /* відкидаємо праві повороти: знаходимо lower_bound */
    return k;                             /* 0 = не знайдено (key > max) */
}

/* Перша (наївна) версія: префетч без обмеження. Адреса через uintptr_t — без UB у C,
 * але апаратно префетч іде на сторінки за межами масиву. Лишаємо для відтворення ефекту. */
static size_t eytz_search_unclamped(const int *b, size_t n, int key) {
    size_t k = 1;
    while (k <= n) {
        __builtin_prefetch((const void *)((uintptr_t)b + k * 16 * sizeof *b));
        k = 2 * k + (size_t)(b[k] < key);
    }
    k >>= __builtin_ffsll((long long)~k);
    return k;
}

static int cmp_int_asc(const void *x, const void *y) {
    int a = *(const int *)x, b = *(const int *)y;
    return (a > b) - (a < b);
}

int main(void) {
    FILE *f = open_csv("results/search_time.csv", "algo,n,bytes,ns_per_query");
    const int MAXLG = 24;
    size_t maxn = (size_t)1 << MAXLG;
    int *a = xmalloc(maxn * sizeof *a), *b = xmalloc((maxn + 1) * sizeof *b);
    int *keys = xmalloc(Q * sizeof *keys);
    for (int lg = 4; lg <= MAXLG; lg++) {
        /* також проміжні точки 1.5*2^k для гладшої кривої */
        for (int half = 0; half < 2; half++) {
            size_t n = ((size_t)1 << lg) + (half ? ((size_t)1 << lg) / 2 : 0);
            if (n > maxn) break;
            /* Відсортовані рівномірно-випадкові ключі. (Перша версія мала a[i] = 2i — арифметичну
             * прогресію, на якій інтерполяція завжди влучає з першої проби; це завищувало її результат.) */
            dsa_rng r;
            dsa_rng_seed(&r, (uint64_t)n);
            for (size_t i = 0; i < n; i++) a[i] = (int)(dsa_rng_next(&r) >> 33);
            qsort(a, n, sizeof *a, cmp_int_asc);
            for (int q = 0; q < Q; q++) keys[q] = a[dsa_rng_below(&r, n)]; /* всі успішні */
            eytz_build(a, b, 0, 1, n);

            const char *names[] = {"linear", "binary", "branchless", "eytzinger", "interpolation", "eytzinger_unclamped"};
            for (int alg = 0; alg < 6; alg++) {
                if (alg == 0 && n > 8192) continue;
                int q_used = alg == 0 ? Q / 64 : Q;
                uint64_t t[5];
                for (int rep = 0; rep < 5; rep++) {
                    long long acc = 0;
                    uint64_t t0 = dsa_now_ns();
                    for (int q = 0; q < q_used; q++) {
                        int key = keys[q];
                        switch (alg) {
                        case 0: acc += search_linear(a, n, key); break;
                        case 1: acc += search_binary(a, n, key); break;
                        case 2: acc += search_branchless(a, n, key); break;
                        case 3: acc += (long long)eytz_search(b, n, key); break;
                        case 4: acc += search_interpolation(a, n, key); break;
                        case 5: acc += (long long)eytz_search_unclamped(b, n, key); break;
                        }
                    }
                    t[rep] = dsa_now_ns() - t0;
                    bench_sink += acc;
                }
                double ns = (double)median_u64(t, 5) / q_used;
                fprintf(f, "%s,%zu,%zu,%.3f\n", names[alg], n, n * sizeof(int), ns);
            }
            /* перевірка коректності eytzinger на вибірці */
            for (int q = 0; q < 1000; q++) {
                size_t k = eytz_search(b, n, keys[q]);
                if (k == 0 || b[k] != keys[q]) { fprintf(stderr, "eytzinger BUG n=%zu\n", n); return 1; }
            }
        }
        fflush(f);
    }
    fclose(f);
    free(a); free(b); free(keys);
    return 0;
}
