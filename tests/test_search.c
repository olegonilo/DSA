#include "dsa_common.h"
#include "dsa_search.h"
#include "minitest.h"

#include <limits.h>
#include <stdlib.h>

typedef ptrdiff_t (*search_fn)(const int *, size_t, int);

static const struct { const char *name; search_fn fn; } fns[] = {
    {"linear", search_linear},
    {"binary", search_binary},
    {"binary_recursive", search_binary_recursive},
    {"branchless", search_branchless},
    {"interpolation", search_interpolation},
    {"jump", search_jump},
    {"exponential", search_exponential},
    {"ternary", search_ternary},
};

/* For an array without duplicates: every present key is found exactly at its index,
 * every absent one (between elements, below min, above max) yields -1. */
static void test_all_searches(void) {
    for (size_t n = 0; n <= 70; n++) {
        int *a = malloc((n ? n : 1) * sizeof *a);
        for (size_t i = 0; i < n; i++) a[i] = (int)(3 * i + 1);
        for (size_t f = 0; f < sizeof fns / sizeof *fns; f++) {
            for (size_t i = 0; i < n; i++) {
                ptrdiff_t r = fns[f].fn(a, n, a[i]);
                if (r != (ptrdiff_t)i) fprintf(stderr, "    %s n=%zu key=%d got %td\n", fns[f].name, n, a[i], r);
                CHECK_EQ_INT(r, i);
            }
            for (int k = -2; k <= (int)(3 * n + 2); k++)
                if (k % 3 != 1 || k < 1 || k > (int)(3 * n - 2)) CHECK_EQ_INT(fns[f].fn(a, n, k), -1);
        }
        int *b = malloc((n ? n : 1) * sizeof *b);
        for (size_t i = 0; i < n; i++) b[i] = a[i];
        for (size_t i = 0; i < n; i++) CHECK_EQ_INT(search_linear_sentinel(b, n, a[i]), i);
        CHECK_EQ_INT(search_linear_sentinel(b, n, -5), -1);
        for (size_t i = 0; i < n; i++) CHECK_EQ_INT(b[i], a[i]); /* sentinel restored */
        free(a);
        free(b);
    }
}

static void test_interpolation_extreme_values(void) {
    /* In the naive version (key-a[lo])*(hi-lo) overflows int. */
    int a[] = {INT_MIN, -1000000000, 0, 1000000000, INT_MAX};
    for (size_t i = 0; i < 5; i++) CHECK_EQ_INT(search_interpolation(a, 5, a[i]), i);
    CHECK_EQ_INT(search_interpolation(a, 5, 5), -1);
}

static void test_lower_bound(void) {
    int a[] = {1, 2, 2, 2, 5, 9};
    CHECK_EQ_INT(search_lower_bound(a, 6, 2), 1);
    CHECK_EQ_INT(search_lower_bound(a, 6, 3), 4);
    CHECK_EQ_INT(search_lower_bound(a, 6, 0), 0);
    CHECK_EQ_INT(search_lower_bound(a, 6, 10), 6);
}

static void test_binary_comparisons_bound(void) {
    /* Iterative binary search: at most floor(log2 n)+1 iterations (2 comparisons each). */
    enum { N = 1 << 20 };
    int *a = malloc(N * sizeof *a);
    for (int i = 0; i < N; i++) a[i] = i;
    uint64_t worst = 0;
    for (int k = -1; k <= N; k += 997) {
        dsa_stats_reset();
        search_binary(a, N, k);
        if (g_stats.cmp > worst) worst = g_stats.cmp;
    }
    CHECK(worst <= 2 * (20 + 1));
    free(a);
}

static void test_mid_overflow_demo(void) {
    int lo = INT_MAX - 10, hi = INT_MAX - 2;
    CHECK(binary_mid_buggy(lo, hi) < 0);              /* "negative index" */
    CHECK_EQ_INT(binary_mid_safe(lo, hi), INT_MAX - 6);
}

int main(void) {
    puts("test_search");
    RUN(test_all_searches);
    RUN(test_interpolation_extreme_values);
    RUN(test_lower_bound);
    RUN(test_binary_comparisons_bound);
    RUN(test_mid_overflow_demo);
    return TEST_SUMMARY();
}
