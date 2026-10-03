/* bench_sort — sorting time: algorithm × input type × n.
 * n = 2^8 .. 2^22. For each (algorithm, input) pair n grows while one run < BUDGET_NS;
 * so quadratic algorithms stop earlier automatically instead of "hanging" the benchmark.
 * Output: results/sort_time.csv */
#include "bench_util.h"
#include "dsa_sort.h"

#define BUDGET_NS 400000000ULL /* 0.4 s per run */
#define MIN_LOG 8
#define MAX_LOG 22

int main(void) {
    FILE *f = open_csv("results/sort_time.csv", "algo,input,n,reps,ns_median,ns_per_elem");
    size_t maxn = (size_t)1 << MAX_LOG;
    int *ref = xmalloc(maxn * sizeof *ref), *a = xmalloc(maxn * sizeof *a);
    dsa_input_kind kinds[] = {IN_RANDOM, IN_SORTED, IN_REVERSED, IN_FEW_UNIQUE, IN_NEARLY_SORTED};

    for (size_t s = 0; s < dsa_sorts_count; s++) {
        const dsa_sort_entry *e = &dsa_sorts[s];
        for (size_t kk = 0; kk < sizeof kinds / sizeof *kinds; kk++) {
            for (int lg = MIN_LOG; lg <= MAX_LOG; lg++) {
                size_t n = (size_t)1 << lg;
                dsa_rng r;
                dsa_rng_seed(&r, 12345 + (uint64_t)lg);
                dsa_fill(ref, n, kinds[kk], &r);
                /* counting sort with keys up to 2^31 needs 16 GB of counters - limit keys to [0, n) */
                if (e->nonneg_only) for (size_t i = 0; i < n; i++) ref[i] = (int)((unsigned)ref[i] % n);
                int reps = n <= 4096 ? 15 : n <= 65536 ? 7 : 3;
                uint64_t t[15];
                for (int k = 0; k < reps; k++) {
                    memcpy(a, ref, n * sizeof *a);
                    uint64_t t0 = dsa_now_ns();
                    e->fn(a, n);
                    t[k] = dsa_now_ns() - t0;
                    bench_sink += a[n / 2];
                    if (t[k] > BUDGET_NS) { reps = k + 1; break; }
                }
                if (!dsa_is_sorted(a, n)) { fprintf(stderr, "BUG: %s not sorted\n", e->name); return 1; }
                uint64_t med = median_u64(t, (size_t)reps);
                fprintf(f, "%s,%s,%zu,%d,%llu,%.3f\n", e->name, dsa_input_name(kinds[kk]), n, reps,
                        (unsigned long long)med, (double)med / (double)n);
                fflush(f);
                if (med > BUDGET_NS / 4) break; /* the next n (×2) would be ×4 for Θ(n^2) - stop */
            }
        }
        printf("  %-18s done\n", e->name);
    }
    fclose(f);
    free(ref);
    free(a);
    return 0;
}
