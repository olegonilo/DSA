/* Конспект, binary search (notebook p.74 і p.77): MID = (BEG + END)/2, mid = (beg + end)/2.
 * Для int-індексів при beg + end > INT_MAX додавання переповнюється — це UB (C11 6.5p5);
 * на практиці (wrap-around) mid стає від'ємним і a[mid] читає пам'ять ПЕРЕД масивом.
 * Ця помилка 9 років жила в java.util.Arrays.binarySearch (Bloch, 2006).
 * Для масивів > 2^30 елементів (4 ГБ int) це реальний сценарій.
 * Виправлення: mid = beg + (end - beg) / 2, або індекси типу size_t + напіввідкритий інтервал. */
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
    puts("\n(beg+end)/2 обчислено через unsigned-додавання + перетворення в int — так поводиться залізо;");
    puts("сам вираз на int у таких випадках є UB, і компілятор має право згенерувати що завгодно.");
    return 0;
}
