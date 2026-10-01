#include "dsa_sort.h"
#include "dsa_common.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef DSA_COUNT
#define C_CMP() (g_stats.cmp++)
#define C_SWAP() (g_stats.swap++)
#define C_MOVE(k) (g_stats.move += (k))
#else
#define C_CMP() ((void)0)
#define C_SWAP() ((void)0)
#define C_MOVE(k) ((void)0)
#endif

/* LT(x, y): x < y з підрахунком порівняння. */
#define LT(x, y) (C_CMP(), (x) < (y))
#define GT(x, y) (C_CMP(), (x) > (y))

static inline void swp(int *a, size_t i, size_t j) {
    if (i == j) return; /* обмін із самим собою — не обмін: не рахуємо і не пишемо */
    C_SWAP();
    int t = a[i]; a[i] = a[j]; a[j] = t;
}

/* ------------------------------------------------------------------ O(n^2) */

void sort_bubble_naive(int *a, size_t n) {
    if (n < 2) return;
    for (size_t i = 0; i < n - 1; i++)
        for (size_t j = 0; j < n - 1 - i; j++)
            if (GT(a[j], a[j + 1])) swp(a, j, j + 1);
}

void sort_bubble(int *a, size_t n) {
    /* Покращення: запам'ятовуємо позицію останнього обміну — все праворуч
     * від неї вже на місці. Якщо обмінів не було — масив відсортований (best O(n)). */
    while (n > 1) {
        size_t last = 0;
        for (size_t j = 1; j < n; j++)
            if (GT(a[j - 1], a[j])) { swp(a, j - 1, j); last = j; }
        n = last;
    }
}

void sort_selection(int *a, size_t n) {
    if (n < 2) return;
    for (size_t i = 0; i < n - 1; i++) {
        size_t m = i;
        for (size_t j = i + 1; j < n; j++)
            if (LT(a[j], a[m])) m = j;
        if (m != i) swp(a, i, m);
    }
}

void sort_insertion(int *a, size_t n) {
    for (size_t i = 1; i < n; i++) {
        int key = a[i];
        size_t j = i;
        while (j > 0 && GT(a[j - 1], key)) {
            a[j] = a[j - 1];
            C_MOVE(1);
            j--;
        }
        a[j] = key;
    }
}

void sort_insertion_binary(int *a, size_t n) {
    for (size_t i = 1; i < n; i++) {
        int key = a[i];
        size_t lo = 0, hi = i;              /* шукаємо upper_bound(key) у [0, i) — стабільно */
        while (lo < hi) {
            size_t mid = lo + (hi - lo) / 2;
            if (GT(a[mid], key)) hi = mid; else lo = mid + 1;
        }
        memmove(a + lo + 1, a + lo, (i - lo) * sizeof *a);
        C_MOVE(i - lo);
        a[lo] = key;
    }
}

/* ------------------------------------------------------------------ Shell */

static void shell_with_gaps(int *a, size_t n, const size_t *gaps, size_t ng) {
    for (size_t g = 0; g < ng; g++) {
        size_t h = gaps[g];
        if (h >= n) continue;
        for (size_t i = h; i < n; i++) {
            int key = a[i];
            size_t j = i;
            while (j >= h && GT(a[j - h], key)) {
                a[j] = a[j - h];
                C_MOVE(1);
                j -= h;
            }
            a[j] = key;
        }
    }
}

void sort_shell(int *a, size_t n) {
    /* Ciura (2001) + розширення множенням на 2.25 */
    size_t gaps[64];
    size_t ng = 0;
    static const size_t ciura[] = {1, 4, 10, 23, 57, 132, 301, 701};
    for (size_t i = 0; i < 8; i++) gaps[ng++] = ciura[i];
    while (gaps[ng - 1] * 9 / 4 < n && ng < 64) { gaps[ng] = gaps[ng - 1] * 9 / 4; ng++; }
    /* у спадному порядку */
    for (size_t i = 0; i < ng / 2; i++) { size_t t = gaps[i]; gaps[i] = gaps[ng - 1 - i]; gaps[ng - 1 - i] = t; }
    shell_with_gaps(a, n, gaps, ng);
}

void sort_shell_halving(int *a, size_t n) {
    size_t gaps[64];
    size_t ng = 0;
    for (size_t h = n / 2; h > 0; h /= 2) gaps[ng++] = h;
    shell_with_gaps(a, n, gaps, ng);
}

/* ------------------------------------------------------------------ Merge */

static void merge(int *a, int *buf, size_t lo, size_t mid, size_t hi) {
    /* зливаємо [lo,mid) і [mid,hi); "<=" робить сортування стабільним */
    size_t i = lo, j = mid, k = lo;
    while (i < mid && j < hi) {
        if (!GT(a[i], a[j])) buf[k++] = a[i++];
        else buf[k++] = a[j++];
    }
    while (i < mid) buf[k++] = a[i++];
    while (j < hi) buf[k++] = a[j++];
    memcpy(a + lo, buf + lo, (hi - lo) * sizeof *a);
    C_MOVE(2 * (hi - lo));
}

static void merge_rec(int *a, int *buf, size_t lo, size_t hi) {
    if (hi - lo < 2) return;
    size_t mid = lo + (hi - lo) / 2;
    merge_rec(a, buf, lo, mid);
    merge_rec(a, buf, mid, hi);
    if (!GT(a[mid - 1], a[mid])) return; /* вже впорядковано: best-case O(n) на відсортованому */
    merge(a, buf, lo, mid, hi);
}

void sort_merge(int *a, size_t n) {
    if (n < 2) return;
    int *buf = malloc(n * sizeof *buf);
    if (!buf) abort();
    merge_rec(a, buf, 0, n);
    free(buf);
}

void sort_merge_buf(int *a, size_t n, int *buf) {
    if (n > 1) merge_rec(a, buf, 0, n);
}

void sort_merge_bottomup(int *a, size_t n) {
    if (n < 2) return;
    int *buf = malloc(n * sizeof *buf);
    if (!buf) abort();
    for (size_t w = 1; w < n; w *= 2)
        for (size_t lo = 0; lo + w < n; lo += 2 * w) {
            size_t mid = lo + w, hi = lo + 2 * w < n ? lo + 2 * w : n;
            merge(a, buf, lo, mid, hi);
        }
    free(buf);
}

/* ------------------------------------------------------------------ Quick */

/* Ломуто, опорний = останній. Класична "підручникова" версія. */
static void quick_lomuto(int *a, ptrdiff_t lo, ptrdiff_t hi) {
    while (lo < hi) {
        int p = a[hi];
        ptrdiff_t i = lo;
        for (ptrdiff_t j = lo; j < hi; j++)
            if (LT(a[j], p)) { swp(a, (size_t)i, (size_t)j); i++; }
        swp(a, (size_t)i, (size_t)hi);
        /* рекурсія в меншу частину — глибина стеку O(log n) навіть у worst-case по часу */
        if (i - lo < hi - i) { quick_lomuto(a, lo, i - 1); lo = i + 1; }
        else { quick_lomuto(a, i + 1, hi); hi = i - 1; }
    }
}

void sort_quick_lomuto_last(int *a, size_t n) {
    if (n > 1) quick_lomuto(a, 0, (ptrdiff_t)n - 1);
}

static size_t hoare_partition(int *a, size_t lo, size_t hi, int p) {
    /* інваріант Хоара: повертає j, lo <= j < hi, всі [lo..j] <= p <= [j+1..hi] */
    size_t i = lo - 1, j = hi + 1; /* lo-1 при lo=0 переповнюється до SIZE_MAX і ++ повертає 0 — визначено для unsigned */
    for (;;) {
        do i++; while (LT(a[i], p));
        do j--; while (GT(a[j], p));
        if (i >= j) return j;
        swp(a, i, j);
    }
}

static void quick_hoare(int *a, size_t lo, size_t hi) {
    while (lo < hi) {
        int p = a[lo + (hi - lo) / 2];
        size_t j = hoare_partition(a, lo, hi, p);
        if (j - lo < hi - j) { quick_hoare(a, lo, j); lo = j + 1; }
        else { quick_hoare(a, j + 1, hi); hi = j; }
    }
}

void sort_quick_hoare_mid(int *a, size_t n) {
    if (n > 1) quick_hoare(a, 0, n - 1);
}

static void insertion_range(int *a, size_t lo, size_t hi) {
    for (size_t i = lo + 1; i <= hi; i++) {
        int key = a[i];
        size_t j = i;
        while (j > lo && GT(a[j - 1], key)) { a[j] = a[j - 1]; C_MOVE(1); j--; }
        a[j] = key;
    }
}

static void quick_m3(int *a, size_t lo, size_t hi) {
    while (hi - lo > 16) {
        size_t mid = lo + (hi - lo) / 2;
        if (LT(a[mid], a[lo])) swp(a, mid, lo);
        if (LT(a[hi], a[lo])) swp(a, hi, lo);
        if (LT(a[hi], a[mid])) swp(a, hi, mid);
        int p = a[mid];
        size_t j = hoare_partition(a, lo, hi, p);
        if (j - lo < hi - j) { quick_m3(a, lo, j); lo = j + 1; }
        else { quick_m3(a, j + 1, hi); hi = j; }
    }
    insertion_range(a, lo, hi);
}

void sort_quick_median3(int *a, size_t n) {
    if (n > 1) quick_m3(a, 0, n - 1);
}

static void quick_3way(int *a, ptrdiff_t lo, ptrdiff_t hi) {
    while (lo < hi) {
        int p = a[lo + (hi - lo) / 2];
        ptrdiff_t lt = lo, i = lo, gt = hi;   /* [lo,lt) < p, [lt,i) == p, (gt,hi] > p */
        while (i <= gt) {
            if (LT(a[i], p)) swp(a, (size_t)lt++, (size_t)i++);
            else if (GT(a[i], p)) swp(a, (size_t)i, (size_t)gt--);
            else i++;
        }
        if (lt - lo < hi - gt) { quick_3way(a, lo, lt - 1); lo = gt + 1; }
        else { quick_3way(a, gt + 1, hi); hi = lt - 1; }
    }
}

void sort_quick_3way(int *a, size_t n) {
    if (n > 1) quick_3way(a, 0, (ptrdiff_t)n - 1);
}

/* ------------------------------------------------------------------ Heap */

static void sift_down(int *a, size_t i, size_t n) {
    int v = a[i];
    for (;;) {
        size_t c = 2 * i + 1;
        if (c >= n) break;
        if (c + 1 < n && LT(a[c], a[c + 1])) c++;
        if (!LT(v, a[c])) break;
        a[i] = a[c]; C_MOVE(1);
        i = c;
    }
    a[i] = v;
}

void sort_heap(int *a, size_t n) {
    if (n < 2) return;
    for (size_t i = n / 2; i-- > 0;) sift_down(a, i, n); /* побудова купи: O(n), не O(n log n) */
    for (size_t end = n - 1; end > 0; end--) {
        swp(a, 0, end);
        sift_down(a, 0, end);
    }
}

/* ------------------------------------------------------------------ non-comparison */

void sort_counting(int *a, size_t n) {
    if (n < 2) return;
    int mx = a[0];
    for (size_t i = 0; i < n; i++) {
        if (a[i] < 0) abort(); /* контракт: тільки невід'ємні ключі (інакше cnt[a[i]] — запис за межі) */
        if (a[i] > mx) mx = a[i];
    }
    size_t k = (size_t)mx + 1;
    size_t *cnt = calloc(k, sizeof *cnt);
    int *out = malloc(n * sizeof *out);
    if (!cnt || !out) abort();
    for (size_t i = 0; i < n; i++) cnt[a[i]]++;
    for (size_t v = 1; v < k; v++) cnt[v] += cnt[v - 1];      /* префіксні суми */
    for (size_t i = n; i-- > 0;) out[--cnt[a[i]]] = a[i];      /* з кінця — стабільно */
    memcpy(a, out, n * sizeof *a);
    C_MOVE(2 * n);
    free(cnt);
    free(out);
}

void sort_radix_lsd(int *a, size_t n) {
    if (n < 2) return;
    for (size_t i = 0; i < n; i++) if (a[i] < 0) abort(); /* контракт: невід'ємні ключі */
    uint32_t *src = (uint32_t *)a;
    uint32_t *buf = malloc(n * sizeof *buf);
    if (!buf) abort();
    uint32_t *dst = buf;
    for (unsigned shift = 0; shift < 32; shift += 8) {
        size_t cnt[257] = {0};
        for (size_t i = 0; i < n; i++) cnt[((src[i] >> shift) & 0xFF) + 1]++;
        for (size_t d = 1; d < 257; d++) cnt[d] += cnt[d - 1];
        for (size_t i = 0; i < n; i++) dst[cnt[(src[i] >> shift) & 0xFF]++] = src[i];
        C_MOVE(n);
        uint32_t *t = src; src = dst; dst = t;
    }
    /* 4 проходи (парне число) — результат знову в a */
    free(buf);
}

void sort_bucket(int *a, size_t n) {
    /* Справжній bucket sort (а не counting sort, як у конспекті): n кошиків,
     * кошик елемента = floor((x - min) * n / (max - min + 1)). Кошики — суцільні
     * відрізки одного буфера (спочатку рахуємо розміри), тож без malloc на кошик.
     * Для рівномірного розподілу очікувано O(n); worst-case (все в одному кошику) Θ(n^2). */
    if (n < 2) return;
    int mn = a[0], mx = a[0];
    for (size_t i = 1; i < n; i++) { if (a[i] < mn) mn = a[i]; if (a[i] > mx) mx = a[i]; }
    if (mn == mx) return;
    uint64_t range = (uint64_t)((int64_t)mx - mn) + 1;
    size_t *start = calloc(n + 1, sizeof *start);
    int *out = malloc(n * sizeof *out);
    if (!start || !out) abort();
#define BUCKET_OF(x) ((size_t)((__uint128_t)((uint64_t)((int64_t)(x) - mn)) * n / range))
    for (size_t i = 0; i < n; i++) start[BUCKET_OF(a[i]) + 1]++;
    for (size_t b = 1; b <= n; b++) start[b] += start[b - 1];
    size_t *fill = malloc(n * sizeof *fill);
    if (!fill) abort();
    memcpy(fill, start, n * sizeof *fill);
    for (size_t i = 0; i < n; i++) out[fill[BUCKET_OF(a[i])]++] = a[i];
    C_MOVE(n);
#undef BUCKET_OF
    for (size_t b = 0; b < n; b++)
        if (start[b + 1] - start[b] > 1) insertion_range(out, start[b], start[b + 1] - 1);
    memcpy(a, out, n * sizeof *a);
    C_MOVE(n);
    free(fill);
    free(start);
    free(out);
}

static int cmp_int(const void *x, const void *y) {
    int a = *(const int *)x, b = *(const int *)y;
    C_CMP();
    return (a > b) - (a < b); /* а не a - b: різниця може переповнитись (UB) */
}

void sort_libc_qsort(int *a, size_t n) { qsort(a, n, sizeof *a, cmp_int); }

const dsa_sort_entry dsa_sorts[] = {
    {"bubble_naive", sort_bubble_naive, 1, 0},
    {"bubble", sort_bubble, 1, 0},
    {"selection", sort_selection, 1, 0},
    {"insertion", sort_insertion, 1, 0},
    {"insertion_binary", sort_insertion_binary, 1, 0},
    {"shell_halving", sort_shell_halving, 0, 0},
    {"shell_ciura", sort_shell, 0, 0},
    {"merge_topdown", sort_merge, 0, 0},
    {"merge_bottomup", sort_merge_bottomup, 0, 0},
    {"quick_lomuto_last", sort_quick_lomuto_last, 1, 0},
    {"quick_hoare_mid", sort_quick_hoare_mid, 0, 0},
    {"quick_median3", sort_quick_median3, 0, 0},
    {"quick_3way", sort_quick_3way, 0, 0},
    {"heap", sort_heap, 0, 0},
    {"counting", sort_counting, 0, 1},
    {"radix_lsd", sort_radix_lsd, 0, 1},
    {"bucket", sort_bucket, 0, 0},
    {"libc_qsort", sort_libc_qsort, 0, 0},
};
const size_t dsa_sorts_count = sizeof dsa_sorts / sizeof dsa_sorts[0];
