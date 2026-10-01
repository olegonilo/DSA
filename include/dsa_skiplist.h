/*
 * dsa_skiplist.h — skip list (розділ 9 конспекту), Pugh 1990.
 * Ймовірність просування на рівень вище — параметр p (класично 1/2 або 1/4),
 * щоб експериментально побачити компроміс "пам'ять ↔ кількість кроків".
 */
#ifndef DSA_SKIPLIST_H
#define DSA_SKIPLIST_H

#include <stddef.h>
#include <stdint.h>

#define SKIP_MAX_LEVEL 32

typedef struct skip_node {
    int key;
    int level;                  /* кількість "поверхів" цього вузла */
    struct skip_node *next[];   /* flexible array member (C99) */
} skip_node;

typedef struct {
    skip_node *head;
    int level;                  /* поточний максимальний рівень */
    size_t size;
    double p;
    uint64_t rng;
    uint64_t steps;             /* лічильник переходів по вказівниках (для бенчмарку) */
    size_t pointers;            /* сумарна кількість next-вказівників (пам'ять) */
} skiplist;

int  skip_init(skiplist *s, double p, uint64_t seed);
void skip_free(skiplist *s);
int  skip_insert(skiplist *s, int key);   /* 1 = вставлено, 0 = вже було, -1 = нема пам'яті */
int  skip_contains(skiplist *s, int key);
int  skip_erase(skiplist *s, int key);    /* 1 = видалено, 0 = не знайдено */

#endif
