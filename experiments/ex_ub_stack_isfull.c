/* The notes, coding question 3 (notebook p.107–108, PDF p.97–98): array-based stack.
 *     int MAXSIZE = 8; int stack[8]; int top = -1;
 *     int isfull() { if (top == MAXSIZE) return 1; ... }      <- should be MAXSIZE - 1
 * With top = 7 (stack full: indices 0..7) isfull() returns 0, push sets top = 8 and writes stack[8] —
 * out of the array's bounds (UB). Also, pop() returns no value in the "stack is empty" branch, yet the result
 * is used — UB (C11 6.9.1p12).
 * Here we do 9 pushes: the notes do only 6, so there the bug stays "dormant". */
#include "exp_util.h"

#define MAXSIZE 8

static int stack_[MAXSIZE];
static int top = -1;

static int isfull_notes(void) { return top == MAXSIZE; }
static int isfull_fixed(void) { return top == MAXSIZE - 1; }

static int (*isfull)(void);

static int push(int data) {
    if (!isfull()) { top = top + 1; stack_[top] = data; return 0; }
    printf("      could not insert data, stack is full\n");
    return -1;
}

static void nine_pushes(void) {
    top = -1;
    for (int i = 1; i <= 9; i++) push(i * 10);
    printf("      top after 9 pushes = %d\n", top);
}

static void run_notes(void) { isfull = isfull_notes; nine_pushes(); }
static void run_fixed(void) { isfull = isfull_fixed; nine_pushes(); }

int main(void) {
    puts("1) isfull: top == MAXSIZE (the notes), 9 pushes into an 8-element stack");
    run_in_child("the notes", run_notes);
    puts("2) isfull: top == MAXSIZE - 1 (fixed)");
    run_in_child("fixed", run_fixed);
    return 0;
}
