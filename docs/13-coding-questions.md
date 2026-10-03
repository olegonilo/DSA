# 13. Coding questions

Notes: notebook p.104–108 (PDF p.94–98). These 5 pages contain 29 findings, 21 of them unambiguous errors.
**None of the three programs compiles as written** (each has at least one compile-error category entry in the registry: P6-07/P6-N2, P6-12…P6-21, P6-31).

## Q1. Salaries below/above 3000 (C)

```c
#include <windows.h>                          // P6-06: not needed and unavailable outside Windows
int main (int argc, char *argv[]}             // P6-07: '}' instead of ')'
...
   {  if (salary [i] < 3000]                  // P6-N2: ']' instead of ')'
      lcount ++;
     else
      gcount ++;                              // P6-08: a salary of EXACTLY 3000 lands in "more than 3000"
   printf ("\n There are {%d} employee with   // P6-09: the braces are printed literally
```

Also, `scanf` is not checked (P6-10), and `getchar()` immediately consumes the leftover `'\n'`, so the program does not wait
for Enter (P6-11). Fixed version:

```c
#include <stdio.h>
#define NUM_EMPLOYEE 10

int main(void) {
    int salary[NUM_EMPLOYEE], less = 0, other = 0;
    for (int i = 0; i < NUM_EMPLOYEE; i++) {
        printf("Enter employee salary %d: ", i + 1);
        if (scanf("%d", &salary[i]) != 1) return 1;
    }
    for (int i = 0; i < NUM_EMPLOYEE; i++) {
        if (salary[i] < 3000) less++;
        else other++;
    }
    printf("%d employee(s) with salary >= 3000\n", other);
    printf("%d employee(s) with salary < 3000\n", less);
    return 0;
}
```

## Q2. Doubly linked list (C++)

clang++ reports ~12 errors. The main ones:

| Problem | ID |
|---|---|
| no `#include <iostream>` | P6-12 |
| `class node`, but `Node` is used | P6-13 |
| `class Linked list` (with a space), `public ;` instead of `public:` | P6-15, P6-16 |
| fields `size`/`head_`, but accessed as `size_`/`head`/`tail` | P6-17 |
| `prepend` has no body; the class is not closed with `};` | P6-19, P6-21 |
| variable declared as `lList`, used as `llist` | P6-N3 |

Logic errors that remain even once the code compiles:

- `append` **does not advance `tail_`** (P6-18). After append(10), append(3), append(1) the list is `10 → 1`, and node 3 is lost (leaked).
- `resetIterator` nulls `tail_` instead of `itr` (P6-20). The next `append` dereferences NULL.
- `next`/`previous` are not initialized in the `Node` constructor (P6-14); there is no destructor (P6-24).

Correct logic for the same operations in C: `dll_push_back`, `dll_push_front`, `dll_unlink` in [`src/list.c`](../src/list.c).

## Q3. Array-based stack (C)

Details with an ASan demonstration: [07-stack-queue](07-stack-queue.md#errors-in-the-notes-pushpop).

| Problem | ID |
|---|---|
| `isfull`: `top == MAXSIZE` instead of `MAXSIZE - 1` → out-of-bounds write to `stack[8]` | P6-25 |
| `int MAXSIZE = 8; int stack[8];` — the size is specified twice; `const int` in C is not usable as the size of a global array either (C11 6.6p6), only `#define` or `enum` | P6-26 |
| `pop()` returns no value in the "empty" branch — UB if the result is used (C11 6.9.1p12) | P6-27 |
| `peek()` on an empty stack reads `stack[-1]` | P6-29 |
| `printf("stack empty : %s\n", isempty() "true", "false");` — missing `?` and `:` | P6-31 |

After fixing the syntax the program prints `15 12 1 9 5 3` (the verifier built and ran it).
