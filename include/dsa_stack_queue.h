/*
 * dsa_stack_queue.h — стек (розділ 10) і черга (розділ 11).
 * Масивні реалізації з динамічним ростом (амортизовано O(1)) та кільцевий буфер.
 */
#ifndef DSA_STACK_QUEUE_H
#define DSA_STACK_QUEUE_H

#include <stddef.h>

/* ---------- стек на динамічному масиві ---------- */
typedef struct {
    int *data;
    size_t size, cap;
    size_t reallocs; /* скільки разів довелося перевиділяти пам'ять */
} stack;

void stack_init(stack *s);
void stack_free(stack *s);
int  stack_push(stack *s, int v);       /* 0 = OK, -1 = нема пам'яті */
int  stack_pop(stack *s, int *out);     /* 0 = OK, -1 = underflow */
int  stack_peek(const stack *s, int *out);
/* Ріст на +K елементів замість ×2 — для експерименту: дає Θ(n^2) копіювань сумарно. */
int  stack_push_grow_linear(stack *s, int v, size_t k);

/* ---------- черга на кільцевому буфері (circular queue) ---------- */
/* Лічильник count замість "залишаємо одну клітинку порожньою" — всі cap клітинок корисні. */
typedef struct {
    int *data;
    size_t head, count, cap;
} cqueue;

int  cq_init(cqueue *q, size_t cap);
void cq_free(cqueue *q);
int  cq_enqueue(cqueue *q, int v);      /* -1 = overflow */
int  cq_dequeue(cqueue *q, int *out);   /* -1 = underflow */
int  cq_is_full(const cqueue *q);
int  cq_is_empty(const cqueue *q);

/* ---------- "наївна" лінійна черга: rear росте, front росте, місце не повертається ---------- */
typedef struct {
    int *data;
    size_t front, rear, cap; /* rear = наступна вільна позиція */
} lqueue;

int  lq_init(lqueue *q, size_t cap);
void lq_free(lqueue *q);
int  lq_enqueue(lqueue *q, int v);      /* -1 коли rear == cap, навіть якщо front > 0 */
int  lq_dequeue(lqueue *q, int *out);

/* ---------- deque на кільцевому буфері ---------- */
typedef struct {
    int *data;
    size_t head, count, cap;
} deque;

int  dq_init(deque *d, size_t cap);
void dq_free(deque *d);
int  dq_push_front(deque *d, int v);
int  dq_push_back(deque *d, int v);
int  dq_pop_front(deque *d, int *out);
int  dq_pop_back(deque *d, int *out);

/* ---------- застосування стеку ---------- */
int  balanced_brackets(const char *s);        /* 1 якщо ()[]{} збалансовані */
/* Інфікс -> постфікс (алгоритм сортувальної станції Дейкстри) для однозначних операндів і + - * / ^ ( ).
 * ^ — правоасоціативний. Повертає 0 = OK, -1 = помилка (out має бути >= 2*strlen(in)+1). */
int  infix_to_postfix(const char *in, char *out, size_t outsz);
/* Обчислення постфіксного виразу з однозначними операндами; -1 у *err при помилці. */
long eval_postfix(const char *postfix, int *err);

#endif
