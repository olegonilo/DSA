/* Конспект, coding question 3 (notebook p.107–108, PDF p.97–98): стек на масиві.
 *     int MAXSIZE = 8; int stack[8]; int top = -1;
 *     int isfull() { if (top == MAXSIZE) return 1; ... }      <- має бути MAXSIZE - 1
 * При top = 7 (стек повний: індекси 0..7) isfull() повертає 0, push робить top = 8 і пише stack[8] —
 * за межами масиву (UB). Також pop() у гілці "stack is empty" не повертає значення, а результат
 * використовується — UB (C11 6.9.1p12).
 * Тут відтворюємо 9 push-ів: у конспекті їх 6, тому баг там "сплячий". */
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
    printf("      top після 9 push = %d\n", top);
}

static void run_notes(void) { isfull = isfull_notes; nine_pushes(); }
static void run_fixed(void) { isfull = isfull_fixed; nine_pushes(); }

int main(void) {
    puts("1) isfull: top == MAXSIZE (конспект), 9 push у стек на 8 елементів");
    run_in_child("конспект", run_notes);
    puts("2) isfull: top == MAXSIZE - 1 (виправлено)");
    run_in_child("виправлено", run_fixed);
    return 0;
}
