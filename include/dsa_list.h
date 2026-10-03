/*
 * dsa_list.h — linked lists (section 8 of the notes).
 * Singly, doubly and circular linked lists.
 */
#ifndef DSA_LIST_H
#define DSA_LIST_H

#include <stddef.h>

/* ---------- singly linked list ---------- */
typedef struct sll_node {
    int data;
    struct sll_node *next;
} sll_node;

typedef struct {
    sll_node *head;
    sll_node *tail; /* the tail makes push_back O(1) instead of O(n) */
    size_t size;
} sll;

void sll_init(sll *l);
void sll_free(sll *l);
int  sll_push_front(sll *l, int v);           /* 0 = OK, -1 = out of memory */
int  sll_push_back(sll *l, int v);
int  sll_insert_at(sll *l, size_t pos, int v); /* pos in [0, size] */
int  sll_pop_front(sll *l, int *out);         /* 0 = OK, -1 = empty */
int  sll_remove_value(sll *l, int v);         /* removes the first occurrence; 0/-1 */
sll_node *sll_find(const sll *l, int v);
void sll_reverse(sll *l);                     /* iterative, O(n) time, O(1) memory */
int  sll_has_cycle(const sll_node *head);     /* Floyd: tortoise and hare */
sll_node *sll_middle(const sll *l);           /* slow/fast pointer */

/* ---------- doubly linked list ---------- */
typedef struct dll_node {
    int data;
    struct dll_node *prev, *next;
} dll_node;

typedef struct {
    dll_node *head, *tail;
    size_t size;
} dll;

void dll_init(dll *l);
void dll_free(dll *l);
int  dll_push_front(dll *l, int v);
int  dll_push_back(dll *l, int v);
int  dll_pop_back(dll *l, int *out);
void dll_unlink(dll *l, dll_node *n);  /* O(1) if the node is known - the main advantage of a DLL */

/* ---------- circular singly linked list (only tail is stored; head = tail->next) ---------- */
typedef struct {
    sll_node *tail;
    size_t size;
} cll;

void cll_init(cll *l);
void cll_free(cll *l);
int  cll_push_back(cll *l, int v);
int  cll_pop_front(cll *l, int *out);
/* Josephus problem: n people, every k-th is eliminated; returns the number (1..n) of the survivor. */
int  josephus_cll(int n, int k);

#endif
