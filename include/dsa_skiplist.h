/*
 * dsa_skiplist.h — skip list (section 9 of the notes), Pugh 1990.
 * The probability of promotion to the next level is a parameter p (classically 1/2 or 1/4),
 * to observe the "memory vs. number of steps" trade-off experimentally.
 */
#ifndef DSA_SKIPLIST_H
#define DSA_SKIPLIST_H

#include <stddef.h>
#include <stdint.h>

#define SKIP_MAX_LEVEL 32

typedef struct skip_node {
    int key;
    int level;                  /* number of "levels" of this node */
    struct skip_node *next[];   /* flexible array member (C99) */
} skip_node;

typedef struct {
    skip_node *head;
    int level;                  /* current maximum level */
    size_t size;
    double p;
    uint64_t rng;
    uint64_t steps;             /* counter of pointer hops (for the benchmark) */
    size_t pointers;            /* total number of next pointers (memory) */
} skiplist;

int  skip_init(skiplist *s, double p, uint64_t seed);
void skip_free(skiplist *s);
int  skip_insert(skiplist *s, int key);   /* 1 = inserted, 0 = already present, -1 = out of memory */
int  skip_contains(skiplist *s, int key);
int  skip_erase(skiplist *s, int key);    /* 1 = removed, 0 = not found */

#endif
