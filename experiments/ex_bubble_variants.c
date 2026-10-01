/* Конспект, bubble sort (notebook p.79–80, PDF p.80–81).
 * (1) Псевдокод — ОДИН прохід "for all array elements: if arr[i] > arr[i+1] swap" — не сортує.
 * (2) C-функція bubble() порівнює a[i] з a[j] для j > i (НЕ сусідні елементи) — це exchange sort
 *     (sort "обміном з мінімумом"), а не bubble sort. Вона сортує правильно, але:
 *       - завжди n(n-1)/2 порівнянь, навіть на відсортованому масиві (немає раннього виходу);
 *       - обмінів рівно стільки ж, скільки в bubble: кожен обмін усуває рівно одну інверсію
 *         (виміряно нижче — однакові числа на всіх входах).
 * Нижче — точні лічильники для трьох версій на однакових входах. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned long long cmp, swp;

static void notes_pseudocode_one_pass(int a[], int n) {
    for (int i = 0; i + 1 < n; i++) { cmp++; if (a[i] > a[i + 1]) { int t = a[i]; a[i] = a[i + 1]; a[i + 1] = t; swp++; } }
}

static void notes_c_exchange(int a[], int n) { /* дослівна логіка з конспекту */
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
    int ex[] = {10, 35, 32, 13, 26}; /* вхід з конспекту */
    int b[5];
    memcpy(b, ex, sizeof ex);
    notes_pseudocode_one_pass(b, 5);
    printf("псевдокод (1 прохід) на {10,35,32,13,26}: ");
    for (int i = 0; i < 5; i++) printf("%d ", b[i]);
    printf("-> %s\n\n", sorted(b, 5) ? "відсортовано" : "НЕ відсортовано");

    enum { N = 2000 };
    static int a[N], src[N];
    const char *inputs[] = {"random", "sorted", "reversed"};
    printf("n = %d          %-22s %-22s\n", N, "exchange (конспект)", "bubble + прапорець");
    printf("%-10s %11s %10s %11s %10s\n", "вхід", "порівнянь", "обмінів", "порівнянь", "обмінів");
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
    puts("\nВисновок: обидва Θ(n^2) у середньому, але лише bubble з прапорцем має best case Θ(n).");
    return 0;
}
