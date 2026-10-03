/*
 * dsa_search.h — searching (sections 16-17 of the notes).
 * All functions return the index of the found element or -1.
 * Functions for sorted arrays require ascending order.
 */
#ifndef DSA_SEARCH_H
#define DSA_SEARCH_H

#include <stddef.h>

ptrdiff_t search_linear(const int *a, size_t n, int key);
ptrdiff_t search_linear_sentinel(int *a, size_t n, int key); /* temporarily modifies a[n-1] */

ptrdiff_t search_binary(const int *a, size_t n, int key);          /* iterative, mid = lo + (hi-lo)/2 */
ptrdiff_t search_binary_recursive(const int *a, size_t n, int key);
ptrdiff_t search_lower_bound(const int *a, size_t n, int key);     /* first index with a[i] >= key (n if none) */
ptrdiff_t search_branchless(const int *a, size_t n, int key);      /* no branches in the loop */
ptrdiff_t search_interpolation(const int *a, size_t n, int key);
ptrdiff_t search_jump(const int *a, size_t n, int key);            /* step floor(sqrt(n)) */
ptrdiff_t search_exponential(const int *a, size_t n, int key);
ptrdiff_t search_ternary(const int *a, size_t n, int key);

/* Bug demo: mid = (lo + hi) / 2 on int - overflows when lo+hi > INT_MAX. */
int binary_mid_buggy(int lo, int hi);
int binary_mid_safe(int lo, int hi);

#endif
