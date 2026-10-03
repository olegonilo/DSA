/* The notes, "bucket sort" (notebook p.83–84, PDF p.84–85):
 *     int max = getmax(a, n);
 *     int bucket[max], i;
 *     for (int i = 0; i <= max; i++) bucket[i] = 0;     <- bucket[max] is out of bounds (size max)
 * Two errors:
 *   (1) the array needs max+1 cells (values 0..max), but is declared with max -> stack-buffer-overflow;
 *   (2) the algorithm is counting sort (per-value counters), not bucket sort (range buckets
 *       + sorting inside each bucket). A real bucket sort: src/sort.c, sort_bucket().
 * Input — exactly from the notes: {54, 12, 84, 57, 69, 41, 9, 5}. */
#include "exp_util.h"

static int getmax(const int a[], int n) {
    int max = a[0];
    for (int i = 1; i < n; i++)
        if (a[i] > max) max = a[i];
    return max;
}

static void notes_counting(int a[], int n) {
    int max = getmax(a, n);
    int bucket[max]; /* VLA of size max: valid indices 0..max-1 */
    for (int i = 0; i <= max; i++) bucket[i] = 0;
    for (int i = 0; i < n; i++) bucket[a[i]]++;
    for (int i = 0, j = 0; i <= max; i++)
        while (bucket[i] > 0) { a[j++] = i; bucket[i]--; }
}

static void fixed_counting(int a[], int n) {
    int max = getmax(a, n);
    int bucket[max + 1];
    for (int i = 0; i <= max; i++) bucket[i] = 0;
    for (int i = 0; i < n; i++) bucket[a[i]]++;
    for (int i = 0, j = 0; i <= max; i++)
        while (bucket[i] > 0) { a[j++] = i; bucket[i]--; }
}

static void run(void (*f)(int[], int)) {
    int a[] = {54, 12, 84, 57, 69, 41, 9, 5};
    f(a, 8);
    printf("      result:");
    for (int i = 0; i < 8; i++) printf(" %d", a[i]);
    printf("\n");
}
static void run_notes(void) { run(notes_counting); }
static void run_fixed(void) { run(fixed_counting); }

int main(void) {
    puts("1) int bucket[max] (the notes)");
    run_in_child("the notes", run_notes);
    puts("2) int bucket[max + 1] (fixed)");
    run_in_child("fixed", run_fixed);
    return 0;
}
