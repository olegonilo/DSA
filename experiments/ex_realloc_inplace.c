/* Чому на графіку stack_growth лінійний ріст через realloc() НЕ дає квадратичного часу?
 * Перевірка: чи змінюється адреса блоку після realloc і скільки це коштує порівняно з
 * "підручниковим" malloc(нового) + memcpy. Між блоком і realloc алокуємо блокер.
 * Результат на macOS (виміряно): великі блоки розширюються НА МІСЦІ (адреса та сама) за мікросекунди —
 * аллокатор виділяє великі блоки окремими регіонами віртуальної пам'яті і просто дорощує регіон.
 * Висновок: асимптотика "Θ(n²) копіювань при рості +K" — властивість моделі, а не закон природи;
 * реальна вартість залежить від аллокатора. Подвоєння (×2) — правильна стратегія в будь-якому разі. */
#include "dsa_common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    printf("%8s  %-10s %14s %26s\n", "розмір", "realloc", "час realloc", "malloc+memcpy того ж розміру");
    for (size_t mb = 1; mb <= 256; mb *= 4) {
        size_t sz = mb << 20;
        char *p = malloc(sz);
        void *blocker = malloc(64);
        if (!p || !blocker) return 1;
        memset(p, 1, sz);
        uintptr_t old = (uintptr_t)p; /* порівнюємо як число: використання p після realloc — UB */
        uint64_t t0 = dsa_now_ns();
        char *q = realloc(p, sz + ((size_t)1 << 20));
        uint64_t t1 = dsa_now_ns();
        if (!q) return 1;
        char *r = malloc(sz);
        if (!r) return 1;
        uint64_t t2 = dsa_now_ns();
        memcpy(r, q, sz);
        uint64_t t3 = dsa_now_ns();
        volatile char sink = r[sz / 2];
        (void)sink;
        printf("%5zu МБ  %-10s %11.1f мкс %23.1f мкс\n", mb, (uintptr_t)q == old ? "на місці" : "ПЕРЕНЕСЕНО",
               (double)(t1 - t0) / 1e3, (double)(t3 - t2) / 1e3);
        free(q); free(r); free(blocker);
    }
    puts("(memcpy включає перший дотик до сторінок нового блоку — page faults; це і є реальна ціна копії)");
    return 0;
}
