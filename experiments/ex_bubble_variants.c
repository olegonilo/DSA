/* The notes, bubble sort (notebook p.79–80, PDF p.80–81).
 * (1) The pseudocode is ONE pass "for all array elements: if arr[i] > arr[i+1] swap" — it does not sort.
 * (2) The C function bubble() compares a[i] with a[j] for j > i (NOT adjacent elements) — this is exchange sort
 *     (sorting by "exchange with the minimum"), not bubble sort. It sorts correctly, but:
 *       - always n(n-1)/2 comparisons, even on a sorted array (no early exit);
 *       - exactly as many swaps as bubble sort: each swap removes exactly one inversion
 *         (measured below — identical numbers on all inputs).
 * Below are exact counters for three versions on identical inputs. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned long long cmp, swp;

static void notes_pseudocode_one_pass(int a[], int n) {
    for (int i = 0; i + 1 < n; i++) { cmp++; if (a[i] > a[i + 1]) { int t = a[i]; a[i] = a[i + 1]; a[i + 1] = t; swp++; } }
}

static void notes_c_exchange(int a[], int n) { /* verbatim logic from the notes */
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            cmp++;
            if (a[j] < a[i]) { int t = a[i]; a[i] = a[j]; a[j] = t; swp++; }
        }
}

static void real_bubble_flag(int a[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            cmp++;
            if (a[j] > a[j + 1]) { int t = a[j]; a[j] = a[j + 1]; a[j + 1] = t; swp++; swapped = 1; }
        }
        if (!swapped) break;
    }
}

static int sorted(const int *a, int n) { for (int i = 1; i < n; i++) if (a[i - 1] > a[i]) return 0; return 1; }

int main(void) {
    int ex[] = {10, 35, 32, 13, 26}; /* input from the notes */
    int b[5];
    memcpy(b, ex, sizeof ex);
    notes_pseudocode_one_pass(b, 5);
    printf("pseudocode (1 pass) on {10,35,32,13,26}: ");
    for (int i = 0; i < 5; i++) printf("%d ", b[i]);
    printf("-> %s\n\n", sorted(b, 5) ? "sorted" : "NOT sorted");

    enum { N = 2000 };
    static int a[N], src[N];
    const char *inputs[] = {"random", "sorted", "reversed"};
    printf("n = %d          %-22s %-22s\n", N, "exchange (the notes)", "bubble + flag");
    printf("%-10s %11s %10s %11s %10s\n", "input", "comparisons", "swaps", "comparisons", "swaps");
    srand(1);
    for (int in = 0; in < 3; in++) {
        for (int i = 0; i < N; i++) src[i] = in == 0 ? rand() : in == 1 ? i : N - i;
        unsigned long long c1, s1;
        memcpy(a, src, sizeof a); cmp = swp = 0; notes_c_exchange(a, N); c1 = cmp; s1 = swp;
        if (!sorted(a, N)) return 1;
        memcpy(a, src, sizeof a); cmp = swp = 0; real_bubble_flag(a, N);
        if (!sorted(a, N)) return 1;
        printf("%-10s %11llu %10llu %11llu %10llu\n", inputs[in], c1, s1, cmp, swp);
    }
    puts("\nConclusion: both are Θ(n^2) on average, but only bubble with a flag has a best case of Θ(n).");
    return 0;
}
