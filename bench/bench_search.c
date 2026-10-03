/* bench_search — time per search (ns) vs n, including n that do not fit in cache.
 * Compared:
 *   linear        — O(n), but sequential access and SIMD vectorization;
 *   binary        — classic O(log n) with branches (~50% mispredicts at every step);
 *   branchless    — the same O(log n), but branch-free (csel/cmov);
 *   eytzinger     — "improving the data structure": array in tree BFS order + prefetch.
 *                   Same asymptotics, but better cache-miss behavior - see Khuong & Morin, 2017;
 *   interpolation — O(log log n) on uniform data.
 * Output: results/search_time.csv */
#include "bench_util.h"
#include "dsa_search.h"

#define Q (1 << 20)

/* Eytzinger: b[1..n] - implicit tree (children of k: 2k, 2k+1), filled by an in-order traversal of a[]. */
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
        /* k*16: prefetch 4 levels ahead (16 int = 64 B - an x86 cache line; on Apple M4 hw.cachelinesize = 128). The index is clamped to n:
         * (1) pointer arithmetic past the array is UB (C11 6.5.6p8);
         * (2) prefetching not-yet-mapped pages causes expensive page walks (measured: 38 ns
         *     instead of ~6 ns at n=1024 in the first version of this benchmark). */
        size_t pf = k * 16;
        pf = pf <= n ? pf : n;
        __builtin_prefetch(b + pf);
        k = 2 * k + (size_t)(b[k] < key);
    }
    k >>= __builtin_ffsll((long long)~k); /* drop the right turns: yields lower_bound */
    return k;                             /* 0 = not found (key > max) */
}

/* First (naive) version: unclamped prefetch. Address via uintptr_t - no UB in C,
 * but the hardware prefetch touches pages beyond the array. Kept to reproduce the effect. */
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
        /* also intermediate points 1.5*2^k for a smoother curve */
        for (int half = 0; half < 2; half++) {
            size_t n = ((size_t)1 << lg) + (half ? ((size_t)1 << lg) / 2 : 0);
            if (n > maxn) break;
            /* Sorted uniformly random keys. (The first version used a[i] = 2i - an arithmetic
             * progression on which interpolation always hits on the first probe; this inflated its result.) */
            dsa_rng r;
            dsa_rng_seed(&r, (uint64_t)n);
            for (size_t i = 0; i < n; i++) a[i] = (int)(dsa_rng_next(&r) >> 33);
            qsort(a, n, sizeof *a, cmp_int_asc);
            for (int q = 0; q < Q; q++) keys[q] = a[dsa_rng_below(&r, n)]; /* all successful */
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
            /* eytzinger correctness check on a sample */
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
