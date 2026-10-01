/* Конспект, "bucket sort" (notebook p.83–84, PDF p.84–85):
 *     int max = getmax(a, n);
 *     int bucket[max], i;
 *     for (int i = 0; i <= max; i++) bucket[i] = 0;     <- bucket[max] — за межами (розмір max)
 * Дві помилки:
 *   (1) масив має max+1 клітинку (значення 0..max), а оголошено max -> stack-buffer-overflow;
 *   (2) алгоритм — це counting sort (лічильники значень), а не bucket sort (кошики-діапазони
 *       + сортування всередині кошика). Справжній bucket sort: src/sort.c, sort_bucket().
 * Вхід — точно з конспекту: {54, 12, 84, 57, 69, 41, 9, 5}. */
#include "exp_util.h"

static int getmax(const int a[], int n) {
    int max = a[0];
    for (int i = 1; i < n; i++)
        if (a[i] > max) max = a[i];
    return max;
}

static void notes_counting(int a[], int n) {
    int max = getmax(a, n);
    int bucket[max]; /* VLA розміру max: допустимі індекси 0..max-1 */
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
    printf("      результат:");
    for (int i = 0; i < 8; i++) printf(" %d", a[i]);
    printf("\n");
}
static void run_notes(void) { run(notes_counting); }
static void run_fixed(void) { run(fixed_counting); }

int main(void) {
    puts("1) int bucket[max] (конспект)");
    run_in_child("конспект", run_notes);
    puts("2) int bucket[max + 1] (виправлено)");
    run_in_child("виправлено", run_fixed);
    return 0;
}
