/*
 * dsa_search.h — пошук (розділи 16–17 конспекту).
 * Всі функції повертають індекс знайденого елемента або -1.
 * Функції для відсортованих масивів вимагають сортування за зростанням.
 */
#ifndef DSA_SEARCH_H
#define DSA_SEARCH_H

#include <stddef.h>

ptrdiff_t search_linear(const int *a, size_t n, int key);
ptrdiff_t search_linear_sentinel(int *a, size_t n, int key); /* тимчасово змінює a[n-1] */

ptrdiff_t search_binary(const int *a, size_t n, int key);          /* ітеративний, mid = lo + (hi-lo)/2 */
ptrdiff_t search_binary_recursive(const int *a, size_t n, int key);
ptrdiff_t search_lower_bound(const int *a, size_t n, int key);     /* перший індекс з a[i] >= key (n якщо нема) */
ptrdiff_t search_branchless(const int *a, size_t n, int key);      /* без розгалужень у циклі */
ptrdiff_t search_interpolation(const int *a, size_t n, int key);
ptrdiff_t search_jump(const int *a, size_t n, int key);            /* крок floor(sqrt(n)) */
ptrdiff_t search_exponential(const int *a, size_t n, int key);
ptrdiff_t search_ternary(const int *a, size_t n, int key);

/* Демонстрація помилки: mid = (lo + hi) / 2 на int — переповнення при lo+hi > INT_MAX. */
int binary_mid_buggy(int lo, int hi);
int binary_mid_safe(int lo, int hi);

#endif
