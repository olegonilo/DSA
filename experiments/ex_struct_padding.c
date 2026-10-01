/* Розділ 6 (structure) + розділ 7 (array): як розташування полів впливає на пам'ять і швидкість.
 *  1) padding: порядок полів змінює sizeof (C11 6.7.2.1p15: поля йдуть у порядку оголошення,
 *     між ними може бути заповнення для вирівнювання);
 *  2) AoS (array of structs) vs SoA (struct of arrays): якщо потрібне одне поле, SoA читає
 *     у кілька разів менше байтів — і це видно на часі, коли дані не влазять у кеш.
 *  Структура employee — з конспекту (notebook p.21): id, name[], mobile (int — помилка: 10-значний
 *  номер > INT_MAX = 2147483647; номер телефону — це рядок, а не число). */
#include "dsa_common.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct employee_notes { int id; char name[20]; int mobile; };          /* як у конспекті */
struct bad_order { char a; double b; char c; int d; char e; };
struct good_order { double b; int d; char a, c, e; };

typedef struct { double salary; int id; char name[52]; } emp_aos;      /* 64 байти = 1 кеш-лінія */

int main(void) {
    printf("sizeof(struct employee_notes) = %zu (id@%zu name@%zu mobile@%zu)\n", sizeof(struct employee_notes),
           offsetof(struct employee_notes, id), offsetof(struct employee_notes, name),
           offsetof(struct employee_notes, mobile));
    printf("sizeof(bad_order)  = %zu  {char, double, char, int, char}\n", sizeof(struct bad_order));
    printf("sizeof(good_order) = %zu  {double, int, char, char, char}\n", sizeof(struct good_order));
    printf("10-значний номер 9876543210 > INT_MAX (%d): у int не влазить\n\n", 2147483647);

    const size_t n = 1u << 20; /* 1М записів: AoS = 64 МБ > L2 (16 МБ) — дані йдуть з DRAM */
    emp_aos *aos = malloc(n * sizeof *aos);
    double *soa_salary = malloc(n * sizeof *soa_salary);
    if (!aos || !soa_salary) return 1;
    for (size_t i = 0; i < n; i++) { aos[i].salary = (double)(i % 1000); aos[i].id = (int)i; soa_salary[i] = aos[i].salary; }
    uint64_t best_aos = UINT64_MAX, best_soa = UINT64_MAX;
    volatile double sink = 0;
    /* 4 незалежні акумулятори: інакше цикл обмежений ЗАТРИМКОЮ FP-додавання (~3 такти на ітерацію,
     * ланцюжок залежностей s += x), а не пам'яттю — і різниця AoS/SoA ховається (виміряно: 1.3x). */
    for (int rep = 0; rep < 7; rep++) {
        double s0 = 0, s1 = 0, s2 = 0, s3 = 0;
        uint64_t t0 = dsa_now_ns();
        for (size_t i = 0; i < n; i += 4) {
            s0 += aos[i].salary; s1 += aos[i + 1].salary; s2 += aos[i + 2].salary; s3 += aos[i + 3].salary;
        }
        uint64_t t1 = dsa_now_ns();
        double r0 = 0, r1 = 0, r2 = 0, r3 = 0;
        for (size_t i = 0; i < n; i += 4) {
            r0 += soa_salary[i]; r1 += soa_salary[i + 1]; r2 += soa_salary[i + 2]; r3 += soa_salary[i + 3];
        }
        uint64_t t2 = dsa_now_ns();
        sink += s0 + s1 + s2 + s3 + r0 + r1 + r2 + r3;
        if (t1 - t0 < best_aos) best_aos = t1 - t0;
        if (t2 - t1 < best_soa) best_soa = t2 - t1;
    }
    printf("Сума зарплат %zu записів (найкращий з 7):\n", n);
    printf("  AoS (64 Б/запис, читаємо %zu МБ): %.2f нс/запис\n", n * sizeof(emp_aos) >> 20, (double)best_aos / (double)n);
    printf("  SoA ( 8 Б/запис, читаємо %zu МБ): %.2f нс/запис\n", n * sizeof(double) >> 20, (double)best_soa / (double)n);
    printf("  прискорення SoA: %.1fx\n", (double)best_aos / (double)best_soa);
    puts("  Увага: це не лише \"у 8 разів менше байтів\". 8 МБ SoA вміщуються в L2 (16 МБ) і після першого\n"
         "  повтору читаються з кешу, а 64 МБ AoS щоразу йдуть з DRAM. Виміряне прискорення = менше байтів + кеш.");
    free(aos);
    free(soa_salary);
    return 0;
}
