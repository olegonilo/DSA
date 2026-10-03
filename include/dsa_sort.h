/*
 * dsa_sort.h — sorting algorithms (sections 18-19 of the notes).
 *
 * All functions sort an int array in ascending order in place (unless noted otherwise).
 * When built with -DDSA_COUNT, the functions count g_stats.cmp / swap / move.
 */
#ifndef DSA_SORT_H
#define DSA_SORT_H

#include <stddef.h>

typedef void (*dsa_sort_fn)(int *a, size_t n);

/* O(n^2) */
void sort_bubble_naive(int *a, size_t n);     /* no early exit - as in most lecture notes */
void sort_bubble(int *a, size_t n);           /* with a swapped flag: best O(n) */
void sort_selection(int *a, size_t n);
void sort_insertion(int *a, size_t n);
void sort_insertion_binary(int *a, size_t n); /* O(n log n) comparisons, but O(n^2) moves */

/* subquadratic */
void sort_shell(int *a, size_t n);            /* Ciura gap sequence */
void sort_shell_halving(int *a, size_t n);    /* n/2, n/4, ... (Shell, 1959) — worst Θ(n^2) */

/* O(n log n) */
void sort_merge(int *a, size_t n);            /* top-down, O(n) buffer */
void sort_merge_buf(int *a, size_t n, int *buf); /* same, with a caller-provided buffer (>= n ints) - no malloc */
void sort_merge_bottomup(int *a, size_t n);
void sort_quick_lomuto_last(int *a, size_t n);/* pivot = last: worst Θ(n^2) on sorted input */
void sort_quick_hoare_mid(int *a, size_t n);  /* Hoare, pivot = middle */
void sort_quick_median3(int *a, size_t n);    /* median-of-3 + insertion for small + recurse into smaller part */
void sort_quick_3way(int *a, size_t n);       /* Dijkstra (Dutch flag): linear on duplicates */
void sort_heap(int *a, size_t n);

/* non-comparison */
void sort_counting(int *a, size_t n);         /* for non-negative ints, O(n + k) */
void sort_radix_lsd(int *a, size_t n);        /* for non-negative ints, 4 passes of 8 bits */
void sort_bucket(int *a, size_t n);           /* n buckets over [min,max] + insertion in each; avg O(n) for uniform input */

/* wrapper around qsort(3) as a baseline */
void sort_libc_qsort(int *a, size_t n);

typedef struct {
    const char *name;
    dsa_sort_fn fn;
    int quadratic;   /* 1 if worst/avg Θ(n^2) - the benchmark limits n */
    int nonneg_only; /* 1 if it accepts only non-negative keys */
} dsa_sort_entry;

extern const dsa_sort_entry dsa_sorts[];
extern const size_t dsa_sorts_count;

#endif
