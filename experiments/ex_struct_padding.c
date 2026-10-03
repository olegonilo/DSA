/* Chapter 6 (structure) + chapter 7 (array): how field layout affects memory and speed.
 *  1) padding: field order changes sizeof (C11 6.7.2.1p15: fields are laid out in declaration order,
 *     with possible padding between them for alignment);
 *  2) AoS (array of structs) vs SoA (struct of arrays): if one field is needed, SoA reads
 *     several times fewer bytes — and this shows up in timing when the data does not fit in cache.
 *  The employee struct is from the notes (notebook p.21): id, name[], mobile (int — an error: a 10-digit
 *  number > INT_MAX = 2147483647; a phone number is a string, not a number). */
#include "dsa_common.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct employee_notes { int id; char name[20]; int mobile; };          /* as in the notes */
struct bad_order { char a; double b; char c; int d; char e; };
struct good_order { double b; int d; char a, c, e; };

typedef struct { double salary; int id; char name[52]; } emp_aos;      /* 64 bytes = 1 cache line */

int main(void) {
    printf("sizeof(struct employee_notes) = %zu (id@%zu name@%zu mobile@%zu)\n", sizeof(struct employee_notes),
           offsetof(struct employee_notes, id), offsetof(struct employee_notes, name),
           offsetof(struct employee_notes, mobile));
    printf("sizeof(bad_order)  = %zu  {char, double, char, int, char}\n", sizeof(struct bad_order));
    printf("sizeof(good_order) = %zu  {double, int, char, char, char}\n", sizeof(struct good_order));
    printf("10-digit number 9876543210 > INT_MAX (%d): does not fit in an int\n\n", 2147483647);

    const size_t n = 1u << 20; /* 1M records: AoS = 64 MB > L2 (16 MB) — data comes from DRAM */
    emp_aos *aos = malloc(n * sizeof *aos);
    double *soa_salary = malloc(n * sizeof *soa_salary);
    if (!aos || !soa_salary) return 1;
    for (size_t i = 0; i < n; i++) { aos[i].salary = (double)(i % 1000); aos[i].id = (int)i; soa_salary[i] = aos[i].salary; }
    uint64_t best_aos = UINT64_MAX, best_soa = UINT64_MAX;
    volatile double sink = 0;
    /* 4 independent accumulators: otherwise the loop is bound by FP-add LATENCY (~3 cycles per iteration,
     * dependency chain s += x), not by memory — and the AoS/SoA difference is hidden (measured: 1.3x). */
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
    printf("Sum of salaries over %zu records (best of 7):\n", n);
    printf("  AoS (64 B/record, reading %zu MB): %.2f ns/record\n", n * sizeof(emp_aos) >> 20, (double)best_aos / (double)n);
    printf("  SoA ( 8 B/record, reading %zu MB): %.2f ns/record\n", n * sizeof(double) >> 20, (double)best_soa / (double)n);
    printf("  SoA speedup: %.1fx\n", (double)best_aos / (double)best_soa);
    puts("  Note: this is not just \"8x fewer bytes\". The 8 MB of SoA fit in L2 (16 MB) and after the first\n"
         "  run are read from cache, while the 64 MB of AoS come from DRAM every time. Measured speedup = fewer bytes + cache.");
    free(aos);
    free(soa_salary);
    return 0;
}
