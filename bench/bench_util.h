/* bench_util.h — методика вимірювання.
 *
 *  - час: CLOCK_MONOTONIC, медіана з R повторів (медіана стійка до викидів від планувальника ОС);
 *  - перед кожним повтором вхід відновлюється з еталонної копії (сортування in-place);
 *  - результат "використовується" через volatile sink, щоб компілятор не викинув обчислення;
 *  - кожен бенчмарк пише CSV у results/ — графіки будуються ТІЛЬКИ з цих файлів.
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
