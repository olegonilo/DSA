/*
 * dsa_sort.h — алгоритми сортування (розділи 18–19 конспекту).
 *
 * Всі функції сортують масив int за зростанням in-place (окрім зазначених).
 * Якщо зібрано з -DDSA_COUNT, функції рахують g_stats.cmp / swap / move.
 */
#ifndef DSA_SORT_H
#define DSA_SORT_H

#include <stddef.h>

typedef void (*dsa_sort_fn)(int *a, size_t n);

/* O(n^2) */
void sort_bubble_naive(int *a, size_t n);     /* без раннього виходу — як у більшості конспектів */
void sort_bubble(int *a, size_t n);           /* з прапорцем swapped: best O(n) */
void sort_selection(int *a, size_t n);
void sort_insertion(int *a, size_t n);
void sort_insertion_binary(int *a, size_t n); /* O(n log n) порівнянь, але O(n^2) переміщень */

/* субквадратичні */
void sort_shell(int *a, size_t n);            /* послідовність Ciura */
void sort_shell_halving(int *a, size_t n);    /* n/2, n/4, ... (Shell, 1959) — worst Θ(n^2) */

/* O(n log n) */
void sort_merge(int *a, size_t n);            /* top-down, O(n) буфер */
void sort_merge_buf(int *a, size_t n, int *buf); /* те саме з буфером викликача (>= n int) — без malloc */
void sort_merge_bottomup(int *a, size_t n);
void sort_quick_lomuto_last(int *a, size_t n);/* опорний = останній: worst Θ(n^2) на відсортованому */
void sort_quick_hoare_mid(int *a, size_t n);  /* Хоара, опорний = середній */
void sort_quick_median3(int *a, size_t n);    /* median-of-3 + insertion для малих + рекурсія в меншу частину */
void sort_quick_3way(int *a, size_t n);       /* Дейкстра (Dutch flag): лінійний на дублікатах */
void sort_heap(int *a, size_t n);

/* не порівняльні */
void sort_counting(int *a, size_t n);         /* для невід'ємних int, O(n + k) */
void sort_radix_lsd(int *a, size_t n);        /* для невід'ємних int, 4 проходи по 8 біт */
void sort_bucket(int *a, size_t n);           /* n кошиків по діапазону [min,max] + insertion у кожному; avg O(n) для рівномірних */

/* обгортка над qsort(3) як еталон */
void sort_libc_qsort(int *a, size_t n);

typedef struct {
    const char *name;
    dsa_sort_fn fn;
    int quadratic;   /* 1 якщо worst/avg Θ(n^2) — бенчмарк обмежує n */
    int nonneg_only; /* 1 якщо приймає тільки невід'ємні ключі */
} dsa_sort_entry;

extern const dsa_sort_entry dsa_sorts[];
extern const size_t dsa_sorts_count;

#endif
