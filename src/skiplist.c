#include "dsa_skiplist.h"

#include <stdlib.h>

static uint64_t xs(uint64_t *s) {
    uint64_t x = *s;
    x ^= x >> 12; x ^= x << 25; x ^= x >> 27;
    *s = x;
    return x * 0x2545F4914F6CDD1DULL;
}

static int random_level(skiplist *s) {
    int lvl = 1;
    /* (x >> 11) * 2^-53 — рівномірне double у [0,1) */
    while (lvl < SKIP_MAX_LEVEL && (double)(xs(&s->rng) >> 11) * 0x1.0p-53 < s->p) lvl++;
    return lvl;
}

static skip_node *node_new(int key, int level) {
    skip_node *n = malloc(sizeof *n + (size_t)level * sizeof n->next[0]);
    if (!n) return NULL;
    n->key = key;
    n->level = level;
    for (int i = 0; i < level; i++) n->next[i] = NULL;
    return n;
}

int skip_init(skiplist *s, double p, uint64_t seed) {
    s->head = node_new(0, SKIP_MAX_LEVEL);
    if (!s->head) return -1;
    s->level = 1;
    s->size = 0;
    s->p = p;
    s->rng = seed ? seed : 1;
    s->steps = 0;
    s->pointers = 0;
    return 0;
}

void skip_free(skiplist *s) {
    skip_node *p = s->head;
    while (p) { skip_node *nx = p->next[0]; free(p); p = nx; }
    s->head = NULL;
}

/* Спуск: на кожному рівні йдемо вправо, поки наступний ключ < key. */
static skip_node *descend(skiplist *s, int key, skip_node **update) {
    skip_node *x = s->head;
    for (int i = s->level - 1; i >= 0; i--) {
        while (x->next[i] && x->next[i]->key < key) { x = x->next[i]; s->steps++; }
        s->steps++; /* крок вниз */
        if (update) update[i] = x;
    }
    return x->next[0];
}

int skip_contains(skiplist *s, int key) {
    skip_node *x = descend(s, key, NULL);
    return x && x->key == key;
}

int skip_insert(skiplist *s, int key) {
    skip_node *update[SKIP_MAX_LEVEL];
    skip_node *x = descend(s, key, update);
    if (x && x->key == key) return 0;
    int lvl = random_level(s);
    if (lvl > s->level) {
        for (int i = s->level; i < lvl; i++) update[i] = s->head;
        s->level = lvl;
    }
    skip_node *n = node_new(key, lvl);
    if (!n) return -1;
    for (int i = 0; i < lvl; i++) {
        n->next[i] = update[i]->next[i];
        update[i]->next[i] = n;
    }
    s->size++;
    s->pointers += (size_t)lvl;
    return 1;
}

int skip_erase(skiplist *s, int key) {
    skip_node *update[SKIP_MAX_LEVEL];
    skip_node *x = descend(s, key, update);
    if (!x || x->key != key) return 0;
    for (int i = 0; i < x->level; i++)
        if (update[i]->next[i] == x) update[i]->next[i] = x->next[i];
    while (s->level > 1 && !s->head->next[s->level - 1]) s->level--;
    s->pointers -= (size_t)x->level;
    free(x);
    s->size--;
    return 1;
}
