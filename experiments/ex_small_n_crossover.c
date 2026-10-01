/* Де Θ(n²) insertion sort перестає бути швидшим за Θ(n log n)?
 * Асимптотика відповіді не дає — тільки вимір. Тому реальні гібридні сортування (introsort, Timsort,
 * наш quick_median3) перемикаються на insertion для малих підмасивів.
 * Метод: масив з 2^18 випадкових int ріжемо на шматки по n і сортуємо кожен; медіана з 7 повторів.
 * merge_topdown викликає malloc/free на кожен шматок — це вартість, а не алгоритм; тому поруч
 * merge_prealloc з одним буфером на весь експеримент (зауваження рев'ю). */
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
    printf("   (нс на елемент)\n");
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
