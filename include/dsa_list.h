/*
 * dsa_list.h — зв'язні списки (розділ 8 конспекту).
 * Однозв'язний (singly), двозв'язний (doubly) і кільцевий (circular) списки.
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
    sll_node *tail; /* хвіст дає push_back за O(1) замість O(n) */
    size_t size;
} sll;

void sll_init(sll *l);
void sll_free(sll *l);
int  sll_push_front(sll *l, int v);           /* 0 = OK, -1 = нема пам'яті */
int  sll_push_back(sll *l, int v);
int  sll_insert_at(sll *l, size_t pos, int v); /* pos в [0, size] */
int  sll_pop_front(sll *l, int *out);         /* 0 = OK, -1 = порожній */
int  sll_remove_value(sll *l, int v);         /* видаляє перше входження; 0/-1 */
sll_node *sll_find(const sll *l, int v);
void sll_reverse(sll *l);                     /* ітеративно, O(n) час, O(1) пам'ять */
int  sll_has_cycle(const sll_node *head);     /* Floyd: черепаха і заєць */
sll_node *sll_middle(const sll *l);           /* повільний/швидкий вказівник */

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
void dll_unlink(dll *l, dll_node *n);  /* O(1) якщо вузол відомий — головна перевага DLL */

/* ---------- circular singly linked list (зберігаємо лише tail; head = tail->next) ---------- */
typedef struct {
    sll_node *tail;
    size_t size;
} cll;

void cll_init(cll *l);
void cll_free(cll *l);
int  cll_push_back(cll *l, int v);
int  cll_pop_front(cll *l, int *out);
/* Задача Йосипа Флавія: n людей, кожен k-й вибуває; повертає номер (1..n) того, хто лишився. */
int  josephus_cll(int n, int k);

#endif
