/*
 * dsa_common.h — спільні утиліти: лічильники операцій, таймер, PRNG.
 *
 * Лічильники (comparisons / swaps / moves) дозволяють міряти не тільки час,
 * а й кількість елементарних операцій — саме те, що описує асимптотика.
 */
#ifndef DSA_COMMON_H
#define DSA_COMMON_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint64_t cmp;   /* порівняння ключів            */
    uint64_t swap;  /* обміни двох елементів        */
    uint64_t move;  /* одиничні записи (копіювання) */
} dsa_stats;

extern dsa_stats g_stats;

static inline void dsa_stats_reset(void) {
    g_stats.cmp = g_stats.swap = g_stats.move = 0;
}

/* Монотонний час у наносекундах (CLOCK_MONOTONIC). */
uint64_t dsa_now_ns(void);

/* xorshift64* — швидкий детермінований PRNG для відтворюваних експериментів. */
typedef struct { uint64_t s; } dsa_rng;
static inline void dsa_rng_seed(dsa_rng *r, uint64_t seed) { r->s = seed ? seed : 0x9E3779B97F4A7C15ULL; }
static inline uint64_t dsa_rng_next(dsa_rng *r) {
    uint64_t x = r->s;
    x ^= x >> 12; x ^= x << 25; x ^= x >> 27;
    r->s = x;
    return x * 0x2545F4914F6CDD1DULL;
}
/* Рівномірне ціле в [0, bound) без modulo bias (метод Леміра). */
static inline uint64_t dsa_rng_below(dsa_rng *r, uint64_t bound) {
    return (uint64_t)(((__uint128_t)dsa_rng_next(r) * bound) >> 64);
}

/* Генератори вхідних даних для бенчмарків. */
typedef enum { IN_RANDOM, IN_SORTED, IN_REVERSED, IN_FEW_UNIQUE, IN_NEARLY_SORTED } dsa_input_kind;
void dsa_fill(int *a, size_t n, dsa_input_kind kind, dsa_rng *r);
const char *dsa_input_name(dsa_input_kind kind);

int dsa_is_sorted(const int *a, size_t n);

#endif
