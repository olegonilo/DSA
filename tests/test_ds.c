/* Tests for linear structures: lists, skip list, stack, queues, stack applications. */
#include "dsa_list.h"
#include "dsa_skiplist.h"
#include "dsa_stack_queue.h"
#include "minitest.h"

#include <string.h>

static void test_sll(void) {
    sll l;
    sll_init(&l);
    for (int i = 0; i < 5; i++) sll_push_back(&l, i);       /* 0 1 2 3 4 */
    sll_push_front(&l, -1);                                  /* -1 0 1 2 3 4 */
    CHECK_EQ_INT(sll_insert_at(&l, 3, 100), 0);              /* -1 0 1 100 2 3 4 */
    CHECK_EQ_INT(sll_insert_at(&l, 99, 1), -1);
    CHECK_EQ_INT(l.size, 7);
    CHECK_EQ_INT(sll_middle(&l)->data, 100);
    CHECK_EQ_INT(sll_remove_value(&l, 4), 0);                /* removing the tail updates tail */
    CHECK_EQ_INT(l.tail->data, 3);
    sll_push_back(&l, 7);
    CHECK_EQ_INT(l.tail->data, 7);
    CHECK_EQ_INT(sll_remove_value(&l, -1), 0);               /* head */
    CHECK_EQ_INT(sll_remove_value(&l, 12345), -1);
    sll_reverse(&l);                                         /* 7 3 2 100 1 0 */
    int expect[] = {7, 3, 2, 100, 1, 0}, k = 0;
    for (sll_node *p = l.head; p; p = p->next) CHECK_EQ_INT(p->data, expect[k++]);
    CHECK_EQ_INT(k, 6);
    CHECK_EQ_INT(l.tail->data, 0);
    CHECK(!sll_has_cycle(l.head));
    l.tail->next = l.head->next->next;                       /* artificial cycle */
    CHECK(sll_has_cycle(l.head));
    l.tail->next = NULL;
    int v;
    while (sll_pop_front(&l, &v) == 0) {}
    CHECK(l.head == NULL && l.tail == NULL && l.size == 0);
    CHECK_EQ_INT(sll_pop_front(&l, &v), -1);
    sll_free(&l);
}

static void test_dll_cll(void) {
    dll d;
    dll_init(&d);
    for (int i = 0; i < 4; i++) dll_push_back(&d, i);
    dll_push_front(&d, -1);
    dll_unlink(&d, d.head->next->next);                      /* remove 1 in O(1) */
    int expect[] = {-1, 0, 2, 3}, k = 0;
    for (dll_node *p = d.head; p; p = p->next) CHECK_EQ_INT(p->data, expect[k++]);
    k = 3;
    for (dll_node *p = d.tail; p; p = p->prev) CHECK_EQ_INT(p->data, expect[k--]);
    int v;
    CHECK_EQ_INT(dll_pop_back(&d, &v), 0);
    CHECK_EQ_INT(v, 3);
    dll_free(&d);

    /* Josephus: known values of J(n,k) */
    CHECK_EQ_INT(josephus_cll(7, 3), 4);
    CHECK_EQ_INT(josephus_cll(41, 3), 31);
    CHECK_EQ_INT(josephus_cll(1, 5), 1);
    CHECK_EQ_INT(josephus_cll(10, 2), 5);
}

static void test_skiplist(void) {
    double ps[] = {0.25, 0.5};
    for (int t = 0; t < 2; t++) {
        skiplist s;
        CHECK_EQ_INT(skip_init(&s, ps[t], 7), 0);
        /* example from the notes: 6, 29, 22, 9, 17, 4 */
        int keys[] = {6, 29, 22, 9, 17, 4};
        for (int i = 0; i < 6; i++) CHECK_EQ_INT(skip_insert(&s, keys[i]), 1);
        CHECK_EQ_INT(skip_insert(&s, 22), 0);                /* duplicate */
        int prev = -1, cnt = 0;
        for (skip_node *x = s.head->next[0]; x; x = x->next[0]) { CHECK(x->key > prev); prev = x->key; cnt++; }
        CHECK_EQ_INT(cnt, 6);
        for (int i = 0; i < 6; i++) CHECK(skip_contains(&s, keys[i]));
        CHECK(!skip_contains(&s, 5));
        CHECK_EQ_INT(skip_erase(&s, 9), 1);
        CHECK_EQ_INT(skip_erase(&s, 9), 0);
        CHECK(!skip_contains(&s, 9));
        for (int i = 0; i < 20000; i++) skip_insert(&s, (i * 7919) % 20011);
        for (int i = 0; i < 20000; i += 3) CHECK(skip_contains(&s, (i * 7919) % 20011));
        /* each level i is a sorted subsequence of level i-1 */
        for (int lv = 0; lv < s.level; lv++) {
            int pk = -1;
            for (skip_node *x = s.head->next[lv]; x; x = x->next[lv]) { CHECK(x->key > pk); CHECK(x->level > lv); pk = x->key; }
        }
        skip_free(&s);
    }
}

static void test_stack_queue(void) {
    stack s;
    stack_init(&s);
    int v;
    CHECK_EQ_INT(stack_pop(&s, &v), -1);                     /* underflow */
    for (int i = 0; i < 1000; i++) stack_push(&s, i);
    CHECK_EQ_INT(s.reallocs, 8);                             /* 8,16,...,1024: log2(1024/8)+1 */
    for (int i = 999; i >= 0; i--) { CHECK_EQ_INT(stack_pop(&s, &v), 0); CHECK_EQ_INT(v, i); }
    stack_free(&s);

    cqueue q;
    cq_init(&q, 3);
    CHECK_EQ_INT(cq_dequeue(&q, &v), -1);
    cq_enqueue(&q, 1); cq_enqueue(&q, 2); cq_enqueue(&q, 3);
    CHECK_EQ_INT(cq_enqueue(&q, 4), -1);                     /* full - ALL 3 cells are used */
    cq_dequeue(&q, &v); CHECK_EQ_INT(v, 1);
    CHECK_EQ_INT(cq_enqueue(&q, 4), 0);                      /* wrap-around */
    for (int e = 2; e <= 4; e++) { cq_dequeue(&q, &v); CHECK_EQ_INT(v, e); }
    CHECK(cq_is_empty(&q));
    cq_free(&q);

    lqueue lq;
    lq_init(&lq, 3);
    lq_enqueue(&lq, 1); lq_enqueue(&lq, 2); lq_enqueue(&lq, 3);
    lq_dequeue(&lq, &v); lq_dequeue(&lq, &v);
    CHECK_EQ_INT(lq_enqueue(&lq, 4), -1);  /* linear queue flaw: 2 free cells, yet "overflow" */
    lq_free(&lq);

    deque d;
    dq_init(&d, 4);
    dq_push_back(&d, 2); dq_push_front(&d, 1); dq_push_back(&d, 3); dq_push_front(&d, 0);
    CHECK_EQ_INT(dq_push_back(&d, 9), -1);
    for (int e = 0; e < 2; e++) { dq_pop_front(&d, &v); CHECK_EQ_INT(v, e); }
    dq_pop_back(&d, &v); CHECK_EQ_INT(v, 3);
    dq_pop_back(&d, &v); CHECK_EQ_INT(v, 2);
    CHECK_EQ_INT(dq_pop_back(&d, &v), -1);
    dq_free(&d);
}

static void test_stack_applications(void) {
    CHECK(balanced_brackets("{[()()]}"));
    CHECK(balanced_brackets(""));
    CHECK(!balanced_brackets("([)]"));
    CHECK(!balanced_brackets("(("));
    CHECK(!balanced_brackets("())"));

    char out[64];
    CHECK_EQ_INT(infix_to_postfix("a+b*c", out, sizeof out), 0);
    CHECK(strcmp(out, "abc*+") == 0);
    CHECK_EQ_INT(infix_to_postfix("(a+b)*c-d/e", out, sizeof out), 0);
    CHECK(strcmp(out, "ab+c*de/-") == 0);
    CHECK_EQ_INT(infix_to_postfix("a^b^c", out, sizeof out), 0);   /* right-associative */
    CHECK(strcmp(out, "abc^^") == 0);
    CHECK_EQ_INT(infix_to_postfix("a-b-c", out, sizeof out), 0);   /* left-associative */
    CHECK(strcmp(out, "ab-c-") == 0);
    CHECK_EQ_INT(infix_to_postfix("(a+b", out, sizeof out), -1);
    CHECK_EQ_INT(infix_to_postfix("a+b)", out, sizeof out), -1);

    int err;
    CHECK_EQ_INT(eval_postfix("231*+9-", &err), -4);
    CHECK_EQ_INT(err, 0);
    CHECK_EQ_INT(eval_postfix("23^2^", &err), 64);
    CHECK_EQ_INT(eval_postfix("2+", &err), 0);
    CHECK_EQ_INT(err, -1);
    CHECK_EQ_INT(infix_to_postfix("2^3^2", out, sizeof out), 0);
    CHECK_EQ_INT(eval_postfix(out, &err), 512);                     /* 2^(3^2), not (2^3)^2=64 */
}

/* Regressions from code review: each test reproduces a defect that was found. */
static void test_review_regressions(void) {
    int err;
    eval_postfix("99*9*9*9*9*9*9*9*9*9*9*9*9*9*9*9*9*9*9*9*9*", &err); /* 9^22 > LONG_MAX: used to be UB */
    CHECK_EQ_INT(err, -1);
    eval_postfix("99^9^", &err);                                       /* (9^9)^9: used to be ~4e8 iterations + UB */
    CHECK_EQ_INT(err, -1);
    eval_postfix("201-^", &err);                                       /* 2^(-1): used to silently return "1" */
    CHECK_EQ_INT(err, -1);
    CHECK_EQ_INT(eval_postfix("29^", &err), 512);
    CHECK_EQ_INT(err, 0);

    stack s;
    stack_init(&s);
    CHECK_EQ_INT(stack_push_grow_linear(&s, 1, 0), -1);                /* k == 0: used to be heap-buffer-overflow */
    stack_free(&s);

    /* Josephus vs. the recurrence J(1)=0, J(m)=(J(m-1)+k) mod m (0-based), including k >> n.
     * Before the fix, k = 10^9 meant 10^9 steps around the circle per elimination. */
    const int ks[] = {1, 2, 3, 7, 1000, 1000000000};
    for (int n = 1; n <= 40; n++)
        for (size_t t = 0; t < sizeof ks / sizeof *ks; t++) {
            long long j = 0;
            for (int m = 2; m <= n; m++) j = (j + ks[t]) % m;
            CHECK_EQ_INT(josephus_cll(n, ks[t]), j + 1);
        }
}

int main(void) {
    puts("test_ds");
    RUN(test_review_regressions);
    RUN(test_sll);
    RUN(test_dll_cll);
    RUN(test_skiplist);
    RUN(test_stack_queue);
    RUN(test_stack_applications);
    return TEST_SUMMARY();
}
