# 13. Задачі з кодом (coding questions)

Конспект: notebook p.104–108 (PDF p.94–98). У цих 5 сторінках 29 зауважень, з них 21 однозначна помилка.
**Жодна з трьох програм не компілюється в написаному вигляді** (кожна має щонайменше одну помилку категорії compile-error в реєстрі: P6-07/P6-N2, P6-12…P6-21, P6-31).

## Q1. Зарплати менше/більше 3000 (C)

```c
#include <windows.h>                          // P6-06: не потрібен і відсутній поза Windows
int main (int argc, char *argv[]}             // P6-07: '}' замість ')'
...
   {  if (salary [i] < 3000]                  // P6-N2: ']' замість ')'
      lcount ++;
     else
      gcount ++;                              // P6-08: зарплата РІВНО 3000 потрапляє в "more than 3000"
   printf ("\n There are {%d} employee with   // P6-09: фігурні дужки надрукуються буквально
```

Також `scanf` не перевіряється (P6-10), а `getchar()` одразу з'їдає залишковий `'\n'`, тож програма не чекає
натискання Enter (P6-11). Виправлена версія:

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

## Q2. Двозв'язний список (C++)

clang++ видає ~12 помилок. Головні:

| Проблема | ID |
|---|---|
| немає `#include <iostream>` | P6-12 |
| `class node`, а використовується `Node` | P6-13 |
| `class Linked list` (з пробілом), `public ;` замість `public:` | P6-15, P6-16 |
| поля `size`/`head_`, а звертання `size_`/`head`/`tail` | P6-17 |
| `prepend` без тіла; клас не закрито `};` | P6-19, P6-21 |
| змінну оголошено `lList`, використовується `llist` | P6-N3 |

Логічні помилки, які лишаються, навіть коли код уже компілюється:

- `append` **не зсуває `tail_`** (P6-18). Після append(10), append(3), append(1) список виходить `10 → 1`, а вузол 3 втрачено (витік).
- `resetIterator` обнуляє `tail_` замість `itr` (P6-20). Наступний `append` розіменує NULL.
- `next`/`previous` у конструкторі `Node` не ініціалізовані (P6-14); немає деструктора (P6-24).

Правильна логіка тих самих операцій мовою C: `dll_push_back`, `dll_push_front`, `dll_unlink` у [`src/list.c`](../src/list.c).

## Q3. Стек на масиві (C)

Детально з демонстрацією ASan: [07-stack-queue](07-stack-queue.md#помилки-в-pushpop-конспекту).

| Проблема | ID |
|---|---|
| `isfull`: `top == MAXSIZE` замість `MAXSIZE - 1` → запис `stack[8]` за межі | P6-25 |
| `int MAXSIZE = 8; int stack[8];` — розмір задано двічі; `const int` у C теж не годиться для розміру глобального масиву (C11 6.6p6), лише `#define` або `enum` | P6-26 |
| `pop()` не повертає значення в гілці «empty» — UB при використанні результату (C11 6.9.1p12) | P6-27 |
| `peek()` на порожньому стеку читає `stack[-1]` | P6-29 |
| `printf("stack empty : %s\n", isempty() "true", "false");` — бракує `?` і `:` | P6-31 |

Після виправлення синтаксису програма друкує `15 12 1 9 5 3` (верифікатор зібрав і запустив її).
