/* The notes, insertion sort (notebook p.89, PDF p.90):
 *     while (j >= 0 && temp <= a[j])      <- "<=" instead of "<"
 * Consequences:
 *   (1) the sort becomes UNSTABLE: an element jumps over equal elements;
 *   (2) on an array of identical elements — Θ(n^2) shifts instead of 0 (the best case turns into the worst).
 * Both are checked on records (key, original index). */
#include <stdio.h>

typedef struct { int key, id; } rec;
static unsigned long long moves;

static void insertion(rec a[], int n, int le) {
    for (int i = 1; i < n; i++) {
        rec t = a[i];
        int j = i - 1;
        while (j >= 0 && (le ? t.key <= a[j].key : t.key < a[j].key)) { a[j + 1] = a[j]; j--; moves++; }
        a[j + 1] = t;
    }
}

int main(void) {
    for (int le = 1; le >= 0; le--) {
        rec r[] = {{2, 0}, {1, 1}, {2, 2}, {1, 3}, {2, 4}};
        moves = 0;
        insertion(r, 5, le);
        printf("%s: ", le ? "temp <= a[j] (the notes)" : "temp <  a[j] (fixed)");
        int stable = 1;
        for (int i = 0; i < 5; i++) {
            printf("(%d,#%d) ", r[i].key, r[i].id);
            if (i && r[i].key == r[i - 1].key && r[i].id < r[i - 1].id) stable = 0;
        }
        printf("-> %s\n", stable ? "stable" : "UNSTABLE");
    }
    printf("\nShifts on an array of n identical keys:\n%8s %16s %16s\n", "n", "<= (the notes)", "< (fixed)");
    static rec eq[20000];
    for (int n = 1000; n <= 16000; n *= 2) {
        unsigned long long m[2];
        for (int le = 1; le >= 0; le--) {
            for (int i = 0; i < n; i++) { eq[i].key = 7; eq[i].id = i; }
            moves = 0;
            insertion(eq, n, le);
            m[le] = moves;
        }
        printf("%8d %16llu %16llu   (n(n-1)/2 = %llu)\n", n, m[1], m[0], (unsigned long long)n * (n - 1) / 2);
    }
    return 0;
}
