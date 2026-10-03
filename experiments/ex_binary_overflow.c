/* The notes, binary search (notebook p.74 and p.77): MID = (BEG + END)/2, mid = (beg + end)/2.
 * For int indices with beg + end > INT_MAX the addition overflows — this is UB (C11 6.5p5);
 * in practice (wrap-around) mid becomes negative and a[mid] reads memory BEFORE the array.
 * This bug lived in java.util.Arrays.binarySearch for 9 years (Bloch, 2006).
 * For arrays of > 2^30 elements (4 GB of int) this is a real scenario.
 * Fix: mid = beg + (end - beg) / 2, or size_t indices + a half-open interval. */
#include "dsa_search.h"

#include <limits.h>
#include <stdio.h>

int main(void) {
    int cases[][2] = {{0, 100}, {1 << 30, (1 << 30) + 10}, {INT_MAX - 10, INT_MAX - 2}, {2000000000, 2100000000}};
    printf("%14s %14s %16s %16s\n", "beg", "end", "(beg+end)/2", "beg+(end-beg)/2");
    for (int i = 0; i < 4; i++) {
        int lo = cases[i][0], hi = cases[i][1];
        printf("%14d %14d %16d %16d\n", lo, hi, binary_mid_buggy(lo, hi), binary_mid_safe(lo, hi));
    }
    puts("\n(beg+end)/2 is computed via unsigned addition + conversion to int — this is what the hardware does;");
    puts("the int expression itself is UB in these cases, and the compiler may generate anything at all.");
    return 0;
}
