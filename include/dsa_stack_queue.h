/*
 * dsa_stack_queue.h — stack (section 10) and queue (section 11).
 * Array-based implementations with dynamic growth (amortized O(1)) and a ring buffer.
 */
#ifndef DSA_STACK_QUEUE_H
#define DSA_STACK_QUEUE_H

#include <stddef.h>

/* ---------- stack on a dynamic array ---------- */
typedef struct {
    int *data;
    size_t size, cap;
    size_t reallocs; /* how many times memory had to be reallocated */
} stack;

void stack_init(stack *s);
void stack_free(stack *s);
int  stack_push(stack *s, int v);       /* 0 = OK, -1 = out of memory */
int  stack_pop(stack *s, int *out);     /* 0 = OK, -1 = underflow */
int  stack_peek(const stack *s, int *out);
/* Growth by +K elements instead of ×2 - for the experiment: Θ(n^2) copies in total. */
int  stack_push_grow_linear(stack *s, int v, size_t k);

/* ---------- queue on a ring buffer (circular queue) ---------- */
/* A count field instead of "keep one cell empty" - all cap cells are usable. */
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

/* ---------- "naive" linear queue: rear grows, front grows, space is never reclaimed ---------- */
typedef struct {
    int *data;
    size_t front, rear, cap; /* rear = next free position */
} lqueue;

int  lq_init(lqueue *q, size_t cap);
void lq_free(lqueue *q);
int  lq_enqueue(lqueue *q, int v);      /* -1 when rear == cap, even if front > 0 */
int  lq_dequeue(lqueue *q, int *out);

/* ---------- deque on a ring buffer ---------- */
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

/* ---------- stack applications ---------- */
int  balanced_brackets(const char *s);        /* 1 if ()[]{} are balanced */
/* Infix -> postfix (Dijkstra's shunting-yard algorithm) for single-character operands and + - * / ^ ( ).
 * ^ is right-associative. Returns 0 = OK, -1 = error (out must be >= 2*strlen(in)+1). */
int  infix_to_postfix(const char *in, char *out, size_t outsz);
/* Evaluates a postfix expression with single-digit operands; -1 in *err on error. */
long eval_postfix(const char *postfix, int *err);

#endif
