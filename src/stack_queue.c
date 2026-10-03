#include "dsa_stack_queue.h"

#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================ stack */

void stack_init(stack *s) { s->data = NULL; s->size = s->cap = 0; s->reallocs = 0; }
void stack_free(stack *s) { free(s->data); stack_init(s); }

static int stack_reserve(stack *s, size_t newcap) {
    /* realloc into a temporary: on failure the old block is not lost (otherwise - a leak) */
    int *p = realloc(s->data, newcap * sizeof *p);
    if (!p) return -1;
    s->data = p;
    s->cap = newcap;
    s->reallocs++;
    return 0;
}

int stack_push(stack *s, int v) {
    if (s->size == s->cap && stack_reserve(s, s->cap ? s->cap * 2 : 8) != 0) return -1;
    s->data[s->size++] = v;
    return 0;
}

int stack_push_grow_linear(stack *s, int v, size_t k) {
    if (k == 0) return -1; /* growing by 0 does not enlarge the buffer - the write would be out of bounds */
    if (s->size == s->cap && stack_reserve(s, s->cap + k) != 0) return -1;
    s->data[s->size++] = v;
    return 0;
}

int stack_pop(stack *s, int *out) {
    if (s->size == 0) return -1;
    s->size--;
    if (out) *out = s->data[s->size];
    return 0;
}

int stack_peek(const stack *s, int *out) {
    if (s->size == 0) return -1;
    *out = s->data[s->size - 1];
    return 0;
}

/* ================================================================ circular queue */

int cq_init(cqueue *q, size_t cap) {
    q->head = q->count = 0;
    q->data = cap && cap <= SIZE_MAX / sizeof *q->data ? malloc(cap * sizeof *q->data) : NULL;
    q->cap = q->data ? cap : 0;
    return (q->data || cap == 0) ? 0 : -1;
}
void cq_free(cqueue *q) { free(q->data); q->data = NULL; q->cap = q->count = 0; }
int cq_is_full(const cqueue *q) { return q->count == q->cap; }
int cq_is_empty(const cqueue *q) { return q->count == 0; }

int cq_enqueue(cqueue *q, int v) {
    if (cq_is_full(q)) return -1;
    q->data[(q->head + q->count) % q->cap] = v;
    q->count++;
    return 0;
}

int cq_dequeue(cqueue *q, int *out) {
    if (cq_is_empty(q)) return -1;
    if (out) *out = q->data[q->head];
    q->head = (q->head + 1) % q->cap;
    q->count--;
    return 0;
}

/* ================================================================ linear queue */

int lq_init(lqueue *q, size_t cap) {
    q->front = q->rear = 0;
    q->data = cap && cap <= SIZE_MAX / sizeof *q->data ? malloc(cap * sizeof *q->data) : NULL;
    q->cap = q->data ? cap : 0;
    return (q->data || cap == 0) ? 0 : -1;
}
void lq_free(lqueue *q) { free(q->data); q->data = NULL; }

int lq_enqueue(lqueue *q, int v) {
    if (q->rear == q->cap) return -1; /* "overflow" even if there are free cells at the front */
    q->data[q->rear++] = v;
    return 0;
}

int lq_dequeue(lqueue *q, int *out) {
    if (q->front == q->rear) return -1;
    if (out) *out = q->data[q->front];
    q->front++;
    return 0;
}

/* ================================================================ deque */

int dq_init(deque *d, size_t cap) {
    d->head = d->count = 0;
    d->data = cap && cap <= SIZE_MAX / sizeof *d->data ? malloc(cap * sizeof *d->data) : NULL;
    d->cap = d->data ? cap : 0;
    return (d->data || cap == 0) ? 0 : -1;
}
void dq_free(deque *d) { free(d->data); d->data = NULL; }

int dq_push_front(deque *d, int v) {
    if (d->count == d->cap) return -1;
    d->head = (d->head + d->cap - 1) % d->cap; /* +cap: to avoid going "negative" with size_t */
    d->data[d->head] = v;
    d->count++;
    return 0;
}

int dq_push_back(deque *d, int v) {
    if (d->count == d->cap) return -1;
    d->data[(d->head + d->count) % d->cap] = v;
    d->count++;
    return 0;
}

int dq_pop_front(deque *d, int *out) {
    if (d->count == 0) return -1;
    if (out) *out = d->data[d->head];
    d->head = (d->head + 1) % d->cap;
    d->count--;
    return 0;
}

int dq_pop_back(deque *d, int *out) {
    if (d->count == 0) return -1;
    d->count--;
    if (out) *out = d->data[(d->head + d->count) % d->cap];
    return 0;
}

/* ================================================================ applications */

int balanced_brackets(const char *s) {
    stack st;
    stack_init(&st);
    int ok = 1;
    for (; *s && ok; s++) {
        char c = *s;
        if (c == '(' || c == '[' || c == '{') {
            if (stack_push(&st, c) != 0) ok = 0;
        } else if (c == ')' || c == ']' || c == '}') {
            int top;
            if (stack_pop(&st, &top) != 0) ok = 0;
            else if ((c == ')' && top != '(') || (c == ']' && top != '[') || (c == '}' && top != '{')) ok = 0;
        }
    }
    if (st.size != 0) ok = 0;
    stack_free(&st);
    return ok;
}

static int prec(char op) {
    switch (op) {
    case '^': return 3;
    case '*': case '/': return 2;
    case '+': case '-': return 1;
    default: return 0;
    }
}

int infix_to_postfix(const char *in, char *out, size_t outsz) {
    stack ops;
    stack_init(&ops);
    size_t k = 0;
    int rc = 0;
#define EMIT(ch) do { if (k + 1 >= outsz) { rc = -1; goto done; } out[k++] = (ch); } while (0)
    for (const char *p = in; *p; p++) {
        char c = *p;
        if (isspace((unsigned char)c)) continue;
        if (isalnum((unsigned char)c)) {
            EMIT(c);
        } else if (c == '(') {
            if (stack_push(&ops, c)) { rc = -1; goto done; }
        } else if (c == ')') {
            int t;
            for (;;) {
                if (stack_pop(&ops, &t)) { rc = -1; goto done; } /* unmatched ')' */
                if (t == '(') break;
                EMIT((char)t);
            }
        } else if (prec(c)) {
            int t;
            /* left-associative: pop ops with prec >= ; right-associative ^: only with prec > */
            while (stack_peek(&ops, &t) == 0 && t != '(' &&
                   (prec((char)t) > prec(c) || (prec((char)t) == prec(c) && c != '^'))) {
                stack_pop(&ops, NULL);
                EMIT((char)t);
            }
            if (stack_push(&ops, c)) { rc = -1; goto done; }
        } else {
            rc = -1;
            goto done;
        }
    }
    {
        int t;
        while (stack_pop(&ops, &t) == 0) {
            if (t == '(') { rc = -1; goto done; } /* unclosed '(' */
            EMIT((char)t);
        }
    }
done:
#undef EMIT
    if (outsz) out[rc == 0 ? k : 0] = '\0';
    stack_free(&ops);
    return rc;
}

long eval_postfix(const char *postfix, int *err) {
    long st[256];
    size_t top = 0;
    *err = 0;
    for (const char *p = postfix; *p; p++) {
        char c = *p;
        if (isspace((unsigned char)c)) continue;
        if (isdigit((unsigned char)c)) {
            if (top == 256) { *err = -1; return 0; }
            st[top++] = c - '0';
            continue;
        }
        if (top < 2) { *err = -1; return 0; }
        long b = st[--top], a = st[--top], r = 0;
        int ovf = 0;
        switch (c) {
        case '+': ovf = __builtin_add_overflow(a, b, &r); break;
        case '-': ovf = __builtin_sub_overflow(a, b, &r); break;
        case '*': ovf = __builtin_mul_overflow(a, b, &r); break;
        case '/':
            if (b == 0 || (a == LONG_MIN && b == -1)) { *err = -1; return 0; }
            r = a / b;
            break;
        case '^': {
            if (b < 0) { *err = -1; return 0; } /* integer power with a negative exponent is undefined */
            long base = a;                       /* exponentiation by squaring: O(log b) multiplications */
            r = 1;
            while (b && !ovf) {
                if (b & 1) ovf = __builtin_mul_overflow(r, base, &r);
                b >>= 1;
                if (b && !ovf) ovf = __builtin_mul_overflow(base, base, &base);
            }
            break;
        }
        default: *err = -1; return 0;
        }
        if (ovf) { *err = -1; return 0; } /* signed overflow is UB, so check explicitly */
        st[top++] = r;
    }
    if (top != 1) { *err = -1; return 0; }
    return st[0];
}
