/* Where does Θ(n²) insertion sort stop being faster than Θ(n log n)?
 * Asymptotics cannot answer this — only measurement can. That is why real hybrid sorts (introsort, Timsort,
 * our quick_median3) switch to insertion sort for small subarrays.
 * Method: an array of 2^18 random ints is cut into chunks of n and each chunk is sorted; median of 7 runs.
 * merge_topdown calls malloc/free for every chunk — that is an overhead, not the algorithm; hence, alongside it,
 * merge_prealloc with a single buffer for the whole experiment (a code-review remark). */
#include "dsa_common.h"
#include "dsa_sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int *g_buf;
static void merge_prealloc(int *a, size_t n) { sort_merge_buf(a, n, g_buf); }

static int cmpu(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}

int main(void) {
    enum { TOTAL = 1 << 18 };
    int *src = malloc(TOTAL * sizeof *src), *a = malloc(TOTAL * sizeof *a);
    g_buf = malloc(TOTAL * sizeof *g_buf);
    if (!src || !a || !g_buf) return 1;
    dsa_rng r;
    dsa_rng_seed(&r, 5);
    dsa_fill(src, TOTAL, IN_RANDOM, &r);
    const struct { const char *name; dsa_sort_fn fn; } algs[] = {
        {"insertion", sort_insertion}, {"merge_topdown", sort_merge}, {"merge_prealloc", merge_prealloc},
        {"heap", sort_heap}, {"quick_hoare_mid", sort_quick_hoare_mid}};
    printf("%6s", "n");
    for (int k = 0; k < 5; k++) printf(" %16s", algs[k].name);
    printf("   (ns per element)\n");
    for (size_t n = 4; n <= 512; n *= 2) {
        printf("%6zu", n);
        for (int k = 0; k < 5; k++) {
            uint64_t t[7];
            for (int rep = 0; rep < 7; rep++) {
                memcpy(a, src, TOTAL * sizeof *a);
                uint64_t t0 = dsa_now_ns();
                for (size_t off = 0; off + n <= TOTAL; off += n) algs[k].fn(a + off, n);
                t[rep] = dsa_now_ns() - t0;
            }
            qsort(t, 7, sizeof t[0], cmpu);
            printf(" %16.2f", (double)t[3] / (double)TOTAL);
        }
        printf("\n");
    }
    free(src); free(a); free(g_buf);
    return 0;
}
