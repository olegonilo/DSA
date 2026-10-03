/*
 * dsa_common.h — shared utilities: operation counters, timer, PRNG.
 *
 * Counters (comparisons / swaps / moves) make it possible to measure not only time
 * but also the number of elementary operations - exactly what asymptotics describe.
 */
#ifndef DSA_COMMON_H
#define DSA_COMMON_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint64_t cmp;   /* key comparisons              */
    uint64_t swap;  /* swaps of two elements        */
    uint64_t move;  /* single writes (copies)       */
} dsa_stats;

extern dsa_stats g_stats;

static inline void dsa_stats_reset(void) {
    g_stats.cmp = g_stats.swap = g_stats.move = 0;
}

/* Monotonic time in nanoseconds (CLOCK_MONOTONIC). */
uint64_t dsa_now_ns(void);

/* xorshift64* — fast deterministic PRNG for reproducible experiments. */
typedef struct { uint64_t s; } dsa_rng;
static inline void dsa_rng_seed(dsa_rng *r, uint64_t seed) { r->s = seed ? seed : 0x9E3779B97F4A7C15ULL; }
static inline uint64_t dsa_rng_next(dsa_rng *r) {
    uint64_t x = r->s;
    x ^= x >> 12; x ^= x << 25; x ^= x >> 27;
    r->s = x;
    return x * 0x2545F4914F6CDD1DULL;
}
/* Uniform integer in [0, bound) without modulo bias (Lemire's method). */
static inline uint64_t dsa_rng_below(dsa_rng *r, uint64_t bound) {
    return (uint64_t)(((__uint128_t)dsa_rng_next(r) * bound) >> 64);
}

/* Input data generators for benchmarks. */
typedef enum { IN_RANDOM, IN_SORTED, IN_REVERSED, IN_FEW_UNIQUE, IN_NEARLY_SORTED } dsa_input_kind;
void dsa_fill(int *a, size_t n, dsa_input_kind kind, dsa_rng *r);
const char *dsa_input_name(dsa_input_kind kind);

int dsa_is_sorted(const int *a, size_t n);

#endif
