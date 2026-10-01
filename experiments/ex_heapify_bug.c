/* Конспект, heap sort (notebook p.87, PDF p.88):
 *     if (largest != 1) { swap(a[i], a[largest]); heapify(a, n, largest); }   <- має бути != i
 * Коли вузол i вже більший за дітей, largest == i. Умова "largest != 1" для i != 1 істинна,
 * обмін a[i] з самим собою нічого не змінює, і heapify викликає себе з тими самими аргументами —
 * нескінченна рекурсія -> переповнення стеку (SIGSEGV). Агент-аудитор отримав exit 139 на коді з конспекту.
 * Тут обмежуємо глибину рекурсії лічильником, щоб показати ефект без падіння. */
#include <stdio.h>

static long depth, max_depth;
#define DEPTH_LIMIT 100000

static int heapify(int a[], int n, int i, int buggy) {
    if (++depth > DEPTH_LIMIT) return -1;
    if (depth > max_depth) max_depth = depth;
    int largest = i, left = 2 * i + 1, right = 2 * i + 2;
    if (left < n && a[left] > a[largest]) largest = left;
    if (right < n && a[right] > a[largest]) largest = right;
    int cond = buggy ? (largest != 1) : (largest != i);
    int rc = 0;
    if (cond) {
        int t = a[i]; a[i] = a[largest]; a[largest] = t;
        rc = heapify(a, n, largest, buggy);
    }
    depth--;
    return rc;
}

static int heap_sort(int a[], int n, int buggy) {
    for (int i = n / 2 - 1; i >= 0; i--)
        if (heapify(a, n, i, buggy)) return -1;
    for (int i = n - 1; i > 0; i--) {
        int t = a[0]; a[0] = a[i]; a[i] = t;
        if (heapify(a, i, 0, buggy)) return -1;
    }
    return 0;
}

int main(void) {
    for (int buggy = 1; buggy >= 0; buggy--) {
        int a[] = {48, 10, 23, 43, 28, 26, 1}; /* вхід з конспекту */
        depth = max_depth = 0;
        int rc = heap_sort(a, 7, buggy);
        printf("%s: ", buggy ? "largest != 1 (конспект)" : "largest != i (виправлено)");
        if (rc) printf("глибина рекурсії перевищила %d -> у реальному коді це stack overflow\n", DEPTH_LIMIT);
        else {
            printf("відсортовано, макс. глибина рекурсії = %ld:", max_depth);
            for (int i = 0; i < 7; i++) printf(" %d", a[i]);
            printf("\n");
        }
    }
    return 0;
}
