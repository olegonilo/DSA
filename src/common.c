#if !defined(__APPLE__)
#define _POSIX_C_SOURCE 200809L
#endif
#include "dsa_common.h"

#include <time.h>

dsa_stats g_stats;

uint64_t dsa_now_ns(void) {
#ifdef __APPLE__
    /* On macOS CLOCK_MONOTONIC has only 1 us resolution (measured: every interval is a multiple of 1000 ns).
     * CLOCK_UPTIME_RAW reads the hardware counter (24 MHz on Apple Silicon, tick ~41.7 ns). */
    return clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
#endif
}

void dsa_fill(int *a, size_t n, dsa_input_kind kind, dsa_rng *r) {
    switch (kind) {
    case IN_RANDOM:
        for (size_t i = 0; i < n; i++) a[i] = (int)(dsa_rng_next(r) >> 33);
        break;
    case IN_SORTED:
        for (size_t i = 0; i < n; i++) a[i] = (int)i;
        break;
    case IN_REVERSED:
        for (size_t i = 0; i < n; i++) a[i] = (int)(n - i);
        break;
    case IN_FEW_UNIQUE:
        for (size_t i = 0; i < n; i++) a[i] = (int)dsa_rng_below(r, 8);
        break;
    case IN_NEARLY_SORTED:
        for (size_t i = 0; i < n; i++) a[i] = (int)i;
        /* ~1% random swaps */
        for (size_t k = 0; k < n / 100 + 1 && n > 1; k++) {
            size_t i = (size_t)dsa_rng_below(r, n), j = (size_t)dsa_rng_below(r, n);
            int t = a[i]; a[i] = a[j]; a[j] = t;
        }
        break;
    }
}

const char *dsa_input_name(dsa_input_kind kind) {
    static const char *names[] = {"random", "sorted", "reversed", "few_unique", "nearly_sorted"};
    return names[kind];
}

int dsa_is_sorted(const int *a, size_t n) {
    for (size_t i = 1; i < n; i++)
        if (a[i - 1] > a[i]) return 0;
    return 1;
}
