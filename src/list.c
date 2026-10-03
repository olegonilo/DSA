#include "dsa_list.h"

#include <stdlib.h>

/* ================================================================ singly */

void sll_init(sll *l) { l->head = l->tail = NULL; l->size = 0; }

void sll_free(sll *l) {
    sll_node *p = l->head;
    while (p) {
        sll_node *nx = p->next; /* save next BEFORE free - otherwise use-after-free */
        free(p);
        p = nx;
    }
    sll_init(l);
}

static sll_node *sll_new(int v, sll_node *next) {
    sll_node *n = malloc(sizeof *n);
    if (n) { n->data = v; n->next = next; }
    return n;
}

int sll_push_front(sll *l, int v) {
    sll_node *n = sll_new(v, l->head);
    if (!n) return -1;
    l->head = n;
    if (!l->tail) l->tail = n;
    l->size++;
    return 0;
}

int sll_push_back(sll *l, int v) {
    sll_node *n = sll_new(v, NULL);
    if (!n) return -1;
    if (l->tail) l->tail->next = n; else l->head = n;
    l->tail = n;
    l->size++;
    return 0;
}

int sll_insert_at(sll *l, size_t pos, int v) {
    if (pos > l->size) return -1;
    if (pos == 0) return sll_push_front(l, v);
    if (pos == l->size) return sll_push_back(l, v);
    sll_node *p = l->head;
    for (size_t i = 0; i + 1 < pos; i++) p = p->next;
    sll_node *n = sll_new(v, p->next);
    if (!n) return -1;
    p->next = n;
    l->size++;
    return 0;
}

int sll_pop_front(sll *l, int *out) {
    if (!l->head) return -1;
    sll_node *n = l->head;
    if (out) *out = n->data;
    l->head = n->next;
    if (!l->head) l->tail = NULL;
    free(n);
    l->size--;
    return 0;
}

int sll_remove_value(sll *l, int v) {
    /* Pointer to pointer: the same code for the head and for the middle of the list. */
    sll_node **pp = &l->head, *prev = NULL;
    while (*pp && (*pp)->data != v) { prev = *pp; pp = &(*pp)->next; }
    if (!*pp) return -1;
    sll_node *dead = *pp;
    *pp = dead->next;
    if (l->tail == dead) l->tail = prev;
    free(dead);
    l->size--;
    return 0;
}

sll_node *sll_find(const sll *l, int v) {
    for (sll_node *p = l->head; p; p = p->next)
        if (p->data == v) return p;
    return NULL;
}

void sll_reverse(sll *l) {
    sll_node *prev = NULL, *cur = l->head;
    l->tail = l->head;
    while (cur) {
        sll_node *nx = cur->next;
        cur->next = prev;
        prev = cur;
        cur = nx;
    }
    l->head = prev;
}

int sll_has_cycle(const sll_node *head) {
    const sll_node *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return 1;
    }
    return 0;
}

sll_node *sll_middle(const sll *l) {
    sll_node *slow = l->head, *fast = l->head;
    while (fast && fast->next) { slow = slow->next; fast = fast->next->next; }
    return slow; /* for even n - the second of the two middles */
}

/* ================================================================ doubly */

void dll_init(dll *l) { l->head = l->tail = NULL; l->size = 0; }

void dll_free(dll *l) {
    dll_node *p = l->head;
    while (p) { dll_node *nx = p->next; free(p); p = nx; }
    dll_init(l);
}

int dll_push_front(dll *l, int v) {
    dll_node *n = malloc(sizeof *n);
    if (!n) return -1;
    n->data = v; n->prev = NULL; n->next = l->head;
    if (l->head) l->head->prev = n; else l->tail = n;
    l->head = n;
    l->size++;
    return 0;
}

int dll_push_back(dll *l, int v) {
    dll_node *n = malloc(sizeof *n);
    if (!n) return -1;
    n->data = v; n->next = NULL; n->prev = l->tail;
    if (l->tail) l->tail->next = n; else l->head = n;
    l->tail = n;
    l->size++;
    return 0;
}

void dll_unlink(dll *l, dll_node *n) {
    if (n->prev) n->prev->next = n->next; else l->head = n->next;
    if (n->next) n->next->prev = n->prev; else l->tail = n->prev;
    free(n);
    l->size--;
}

int dll_pop_back(dll *l, int *out) {
    if (!l->tail) return -1;
    if (out) *out = l->tail->data;
    dll_unlink(l, l->tail);
    return 0;
}

/* ================================================================ circular */

void cll_init(cll *l) { l->tail = NULL; l->size = 0; }

void cll_free(cll *l) {
    int dummy;
    while (cll_pop_front(l, &dummy) == 0) {}
}

int cll_push_back(cll *l, int v) {
    sll_node *n = malloc(sizeof *n);
    if (!n) return -1;
    n->data = v;
    if (!l->tail) n->next = n;
    else { n->next = l->tail->next; l->tail->next = n; }
    l->tail = n;
    l->size++;
    return 0;
}

int cll_pop_front(cll *l, int *out) {
    if (!l->tail) return -1;
    sll_node *head = l->tail->next;
    if (out) *out = head->data;
    if (head == l->tail) l->tail = NULL;
    else l->tail->next = head->next;
    free(head);
    l->size--;
    return 0;
}

int josephus_cll(int n, int k) {
    if (n <= 0 || k <= 0) return -1;
    cll c;
    cll_init(&c);
    for (int i = 1; i <= n; i++)
        if (cll_push_back(&c, i) != 0) { cll_free(&c); return -1; }
    sll_node *prev = c.tail;
    while (c.size > 1) {
        /* k mod size: steps around a circle of length size are periodic - O(min(k, size)) instead of O(k) */
        size_t steps = (size_t)(k - 1) % c.size;
        for (size_t s = 0; s < steps; s++) prev = prev->next;
        sll_node *dead = prev->next;
        prev->next = dead->next;
        if (dead == c.tail) c.tail = prev;
        free(dead);
        c.size--;
    }
    int r = c.tail->data;
    cll_free(&c);
    return r;
}
