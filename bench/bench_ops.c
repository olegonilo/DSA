/* bench_ops — ТОЧНА кількість операцій (порівняння / обміни / переміщення).
 * Збирається з -DDSA_COUNT. На відміну від часу, ці числа детерміновані і не залежать від машини:
 * саме їх описують O/Ω/Θ. Вихід: results/sort_ops.csv, results/search_ops.csv */
#include "bench_util.h"
#include "dsa_search.h"
#include "dsa_sort.h"

#include <math.h>

static void sort_ops(void) {
    FILE *f = open_csv("results/sort_ops.csv", "algo,input,n,cmp,swap,move");
    dsa_input_kind kinds[] = {IN_RANDOM, IN_SORTED, IN_REVERSED, IN_FEW_UNIQUE};
    size_t maxn = (size_t)1 << 18;
    int *a = xmalloc(maxn * sizeof *a);
    for (size_t s = 0; s < dsa_sorts_count; s++) {
        const dsa_sort_entry *e = &dsa_sorts[s];
        if (e->nonneg_only) continue; /* не порівняльні: cmp = 0 за визначенням */
        for (size_t kk = 0; kk < sizeof kinds / sizeof *kinds; kk++) {
            for (int lg = 4; lg <= 18; lg++) {
                size_t n = (size_t)1 << lg;
                if (e->quadratic && lg > 14) break;
                dsa_rng r;
                dsa_rng_seed(&r, 999 + (uint64_t)lg);
                dsa_fill(a, n, kinds[kk], &r);
                dsa_stats_reset();
                e->fn(a, n);
                fprintf(f, "%s,%s,%zu,%llu,%llu,%llu\n", e->name, dsa_input_name(kinds[kk]), n,
                        (unsigned long long)g_stats.cmp, (unsigned long long)g_stats.swap,
                        (unsigned long long)g_stats.move);
            }
        }
    }
    fclose(f);
    free(a);
}

typedef ptrdiff_t (*search_fn)(const int *, size_t, int);

static int cmp_int(const void *x, const void *y) {
    int a = *(const int *)x, b = *(const int *)y;
    return (a > b) - (a < b);
}

/* Метрика: кожне порівняння ключа з елементом масиву рахується як 1 (C_CMP у src/search.c).
 * Тому binary робить 2 на ітерацію (== і <), interpolation — до 3 на пробу. Усі алгоритми
 * отримують ОДНАКОВИЙ набір ключів для кожного n (раніше вибірка відрізнялась між алгоритмами). */
static void search_ops(void) {
    FILE *f = open_csv("results/search_ops.csv", "algo,data,n,avg_cmp,max_cmp");
    static const struct { const char *name; search_fn fn; } fns[] = {
        {"linear", search_linear},           {"binary", search_binary},
        {"interpolation", search_interpolation}, {"jump", search_jump},
        {"exponential", search_exponential}, {"ternary", search_ternary},
    };
    static const char *dnames[] = {"arithmetic", "uniform_random", "skewed_x4"};
    size_t maxn = (size_t)1 << 20;
    enum { Q = 2000 };
    int *a = xmalloc(maxn * sizeof *a);
    size_t idx[Q];
    for (int data = 0; data < 3; data++) {
        for (int lg = 4; lg <= 20; lg += 2) {
            size_t n = (size_t)1 << lg;
            dsa_rng r;
            dsa_rng_seed(&r, 7 + (uint64_t)lg);
            if (data == 0) {                     /* арифметична прогресія: інтерполяція вгадує з 1 проби */
                for (size_t i = 0; i < n; i++) a[i] = (int)(i * 4 + 1);
            } else if (data == 1) {              /* відсортовані рівномірно-випадкові ключі в [0, 2^31) */
                for (size_t i = 0; i < n; i++) a[i] = (int)(dsa_rng_next(&r) >> 33);
                qsort(a, n, sizeof *a, cmp_int);
            } else {                             /* x^4: щільно зліва, рідко справа */
                for (size_t i = 0; i < n; i++) a[i] = (int)(pow((double)i / (double)n, 4.0) * 2e9) + (int)i;
            }
            for (int q = 0; q < Q; q++) idx[q] = (size_t)dsa_rng_below(&r, n);
            for (size_t fi = 0; fi < sizeof fns / sizeof *fns; fi++) {
                if (fns[fi].fn == search_linear && lg > 14) continue;
                uint64_t total = 0, mx = 0;
                for (int q = 0; q < Q; q++) {
                    int key = a[idx[q]];             /* успішні пошуки */
                    dsa_stats_reset();
                    if (fns[fi].fn(a, n, key) < 0) { fprintf(stderr, "BUG %s\n", fns[fi].name); exit(1); }
                    total += g_stats.cmp;
                    if (g_stats.cmp > mx) mx = g_stats.cmp;
                }
                fprintf(f, "%s,%s,%zu,%.3f,%llu\n", fns[fi].name, dnames[data], n, (double)total / (double)Q,
                        (unsigned long long)mx);
            }
        }
    }
    fclose(f);
    free(a);
}

int main(void) {
    sort_ops();
    puts("  sort_ops done");
    search_ops();
    puts("  search_ops done");
    return 0;
}
