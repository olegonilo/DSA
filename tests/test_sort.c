#include "dsa_common.h"
#include "dsa_sort.h"
#include "minitest.h"

#include <stdlib.h>
#include <string.h>

static int cmp(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

/* Every algorithm × every input type × sizes, including edge cases 0, 1, 2. */
static void test_all_sorts_match_reference(void) {
    static const size_t sizes[] = {0, 1, 2, 3, 7, 16, 17, 100, 1000, 4099};
    dsa_rng r;
    dsa_rng_seed(&r, 42);
    for (size_t s = 0; s < dsa_sorts_count; s++) {
        for (size_t si = 0; si < sizeof sizes / sizeof *sizes; si++) {
            size_t n = sizes[si];
            for (int kind = IN_RANDOM; kind <= IN_NEARLY_SORTED; kind++) {
                int *a = malloc((n ? n : 1) * sizeof *a), *ref = malloc((n ? n : 1) * sizeof *ref);
                dsa_fill(a, n, (dsa_input_kind)kind, &r);
                if (dsa_sorts[s].nonneg_only && strcmp(dsa_sorts[s].name, "counting") == 0)
                    for (size_t i = 0; i < n; i++) a[i] %= 100000; /* counting: k is bounded */
                memcpy(ref, a, n * sizeof *a);
                qsort(ref, n, sizeof *ref, cmp);
                dsa_sorts[s].fn(a, n);
                int ok = n == 0 || memcmp(a, ref, n * sizeof *a) == 0;
                if (!ok) fprintf(stderr, "    %s n=%zu %s\n", dsa_sorts[s].name, n, dsa_input_name((dsa_input_kind)kind));
                CHECK(ok);
                free(a);
                free(ref);
            }
        }
    }
}

static void test_negative_keys(void) {
    int a[] = {5, -3, 0, -2147483647 - 1, 2147483647, -1, 7, -3};
    int ref[] = {-2147483647 - 1, -3, -3, -1, 0, 5, 7, 2147483647};
    for (size_t s = 0; s < dsa_sorts_count; s++) {
        if (dsa_sorts[s].nonneg_only) continue;
        int b[8];
        memcpy(b, a, sizeof a);
        dsa_sorts[s].fn(b, 8);
        if (memcmp(b, ref, sizeof b) != 0) fprintf(stderr, "    %s failed on INT_MIN/INT_MAX\n", dsa_sorts[s].name);
        CHECK(memcmp(b, ref, sizeof b) == 0);
    }
}

/* Analytical comparison counts - verify that the counters are exact. */
static void test_comparison_counts(void) {
    enum { N = 100 };
    int a[N];
    for (int i = 0; i < N; i++) a[i] = i;

    dsa_stats_reset();
    sort_bubble_naive(a, N);
    CHECK_EQ_INT(g_stats.cmp, N * (N - 1) / 2);   /* naive bubble: always n(n-1)/2 */
    CHECK_EQ_INT(g_stats.swap, 0);

    dsa_stats_reset();
    sort_bubble(a, N);
    CHECK_EQ_INT(g_stats.cmp, N - 1);             /* with early exit: best-case n-1 */

    dsa_stats_reset();
    sort_selection(a, N);
    CHECK_EQ_INT(g_stats.cmp, N * (N - 1) / 2);   /* selection: n(n-1)/2 REGARDLESS of input */

    dsa_stats_reset();
    sort_insertion(a, N);
    CHECK_EQ_INT(g_stats.cmp, N - 1);             /* insertion on sorted: n-1 */

    for (int i = 0; i < N; i++) a[i] = N - i;
    dsa_stats_reset();
    sort_insertion(a, N);
    CHECK_EQ_INT(g_stats.cmp, N * (N - 1) / 2);   /* on reversed: n(n-1)/2 */
    CHECK_EQ_INT(g_stats.move, N * (N - 1) / 2);

    for (int i = 0; i < N; i++) a[i] = N - i;
    dsa_stats_reset();
    sort_bubble_naive(a, N);
    CHECK_EQ_INT(g_stats.swap, N * (N - 1) / 2);  /* number of inversions of a reversed array */

    for (int i = 0; i < N; i++) a[i] = N - i;
    dsa_stats_reset();
    sort_selection(a, N);
    CHECK(g_stats.swap <= N - 1);                 /* selection: at most n-1 swaps */
}

int main(void) {
    puts("test_sort");
    RUN(test_all_sorts_match_reference);
    RUN(test_negative_keys);
    RUN(test_comparison_counts);
    return TEST_SUMMARY();
}
