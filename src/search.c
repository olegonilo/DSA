#include "dsa_search.h"
#include "dsa_common.h"

#include <math.h>
#include <stdint.h>

#ifdef DSA_COUNT
#define C_CMP() (g_stats.cmp++)
#else
#define C_CMP() ((void)0)
#endif

ptrdiff_t search_linear(const int *a, size_t n, int key) {
    for (size_t i = 0; i < n; i++) {
        C_CMP();
        if (a[i] == key) return (ptrdiff_t)i;
    }
    return -1;
}

ptrdiff_t search_linear_sentinel(int *a, size_t n, int key) {
    /* Сторож: кладемо key у кінець — у циклі одна перевірка замість двох (i<n та a[i]==key). */
    if (n == 0) return -1;
    int last = a[n - 1];
    a[n - 1] = key;
    size_t i = 0;
    while (C_CMP(), a[i] != key) i++;
    a[n - 1] = last;
    if (i < n - 1 || last == key) return (ptrdiff_t)i;
    return -1;
}

ptrdiff_t search_binary(const int *a, size_t n, int key) {
    size_t lo = 0, hi = n;               /* напіввідкритий інтервал [lo, hi) — без -1 і без underflow */
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2; /* не (lo+hi)/2 — див. experiments/ex_binary_overflow.c */
        C_CMP();
        if (a[mid] == key) return (ptrdiff_t)mid;
        C_CMP();
        if (a[mid] < key) lo = mid + 1;
        else hi = mid;
    }
    return -1;
}

static ptrdiff_t bin_rec(const int *a, size_t lo, size_t hi, int key) {
    if (lo >= hi) return -1;
    size_t mid = lo + (hi - lo) / 2;
    C_CMP();
    if (a[mid] == key) return (ptrdiff_t)mid;
    C_CMP();
    return a[mid] < key ? bin_rec(a, mid + 1, hi, key) : bin_rec(a, lo, mid, key);
}

ptrdiff_t search_binary_recursive(const int *a, size_t n, int key) { return bin_rec(a, 0, n, key); }

ptrdiff_t search_lower_bound(const int *a, size_t n, int key) {
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        C_CMP();
        if (a[mid] < key) lo = mid + 1; else hi = mid;
    }
    return (ptrdiff_t)lo;
}

ptrdiff_t search_branchless(const int *a, size_t n, int key) {
    /* Кількість ітерацій фіксована (ceil(log2 n)), тіло компілюється у cmov/csel —
     * немає mispredict-ів гілок. Див. bench_search: виграш на масивах, що влазять у кеш. */
    if (n == 0) return -1;
    const int *base = a;
    size_t len = n;
    while (len > 1) {
        size_t half = len / 2;
        C_CMP();
        base = (base[half - 1] < key) ? base + half : base;
        len -= half;
    }
    C_CMP();
    return (*base == key) ? (ptrdiff_t)(base - a) : -1;
}

ptrdiff_t search_interpolation(const int *a, size_t n, int key) {
    if (n == 0) return -1;
    size_t lo = 0, hi = n - 1;
    while (lo <= hi && key >= a[lo] && key <= a[hi]) {
        C_CMP();
        if (a[hi] == a[lo]) return a[lo] == key ? (ptrdiff_t)lo : -1; /* захист від ділення на 0 */
        /* 64-бітна арифметика: (key-a[lo])*(hi-lo) переповнює int вже при ~46341^2 */
        int64_t num = ((int64_t)key - a[lo]) * (int64_t)(hi - lo);
        size_t pos = lo + (size_t)(num / ((int64_t)a[hi] - a[lo]));
        C_CMP();
        if (a[pos] == key) return (ptrdiff_t)pos;
        C_CMP();
        if (a[pos] < key) lo = pos + 1;
        else { if (pos == 0) break; hi = pos - 1; }
    }
    return -1;
}

ptrdiff_t search_jump(const int *a, size_t n, int key) {
    if (n == 0) return -1;
    size_t step = (size_t)sqrt((double)n);
    if (step == 0) step = 1;
    size_t prev = 0, cur = step;
    while (cur < n && (C_CMP(), a[cur - 1] < key)) { prev = cur; cur += step; }
    if (cur > n) cur = n;
    for (size_t i = prev; i < cur; i++) {
        C_CMP();
        if (a[i] == key) return (ptrdiff_t)i;
        if (a[i] > key) break;
    }
    return -1;
}

ptrdiff_t search_exponential(const int *a, size_t n, int key) {
    if (n == 0) return -1;
    C_CMP();
    if (a[0] == key) return 0;
    size_t bound = 1;
    while (bound < n && (C_CMP(), a[bound] < key)) bound *= 2;
    size_t lo = bound / 2, hi = bound + 1 < n ? bound + 1 : n;
    ptrdiff_t r = search_binary(a + lo, hi - lo, key);
    return r < 0 ? -1 : (ptrdiff_t)lo + r;
}

ptrdiff_t search_ternary(const int *a, size_t n, int key) {
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t third = (hi - lo) / 3;
        size_t m1 = lo + third, m2 = hi - 1 - third;
        C_CMP(); if (a[m1] == key) return (ptrdiff_t)m1;
        C_CMP(); if (a[m2] == key) return (ptrdiff_t)m2;
        C_CMP();
        if (key < a[m1]) hi = m1;
        else if (C_CMP(), key > a[m2]) lo = m2 + 1;
        else { lo = m1 + 1; hi = m2; }
    }
    return -1;
}

int binary_mid_buggy(int lo, int hi) {
    /* Відтворюємо те, що робить "int mid = (lo+hi)/2" на апаратному рівні, без UB:
     * беззнакове додавання (модульне, 6.2.5p9) + перетворення в int (implementation-defined,
     * у clang/gcc — wrap-around). Сам вираз lo+hi на int при переповненні — UB (C11 6.5p5). */
    unsigned s = (unsigned)lo + (unsigned)hi;
    return (int)s / 2;
}

int binary_mid_safe(int lo, int hi) { return lo + (hi - lo) / 2; }
