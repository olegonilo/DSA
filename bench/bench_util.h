/* bench_util.h — measurement methodology.
 *
 *  - time: CLOCK_MONOTONIC, median of R repetitions (the median is robust to OS-scheduler outliers);
 *  - before each repetition the input is restored from a reference copy (sorting is in place);
 *  - the result is "used" via a volatile sink so the compiler cannot eliminate the computation;
 *  - every benchmark writes CSV into results/ - charts are built ONLY from these files.
 */
#ifndef BENCH_UTIL_H
#define BENCH_UTIL_H

#include "dsa_common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static volatile long long bench_sink;

static inline int u64_cmp(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}

static inline uint64_t median_u64(uint64_t *v, size_t n) {
    qsort(v, n, sizeof *v, u64_cmp);
    return n % 2 ? v[n / 2] : (v[n / 2 - 1] + v[n / 2]) / 2;
}

static inline FILE *open_csv(const char *path, const char *header) {
    FILE *f = fopen(path, "w");
    if (!f) { perror(path); exit(1); }
    fprintf(f, "%s\n", header);
    return f;
}

static inline void *xmalloc(size_t n) {
    void *p = malloc(n ? n : 1);
    if (!p) { fprintf(stderr, "out of memory (%zu bytes)\n", n); exit(1); }
    return p;
}

#endif
