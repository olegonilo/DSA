# 03. Pointers and structures

Notes: notebook p.18–22 (PDF p.19–23). Errors: chapters 03–04 in [`errata/ERRATA-full.md`](../errata/ERRATA-full.md).

## Pointer — the exact definition

A pointer is **a variable whose value is the address of an object of a particular type** (C11 6.2.5p20).
The notes: "used to points the address of the value" (P2-N1) — vague both grammatically and in meaning.

```
  int a = 10;        int *b = &a;        int **c = &b;
  address 2000       address 3000        address 4000
 ┌──────────┐       ┌──────────┐        ┌──────────┐
 │    10    │ ◄──── │   2000   │ ◄───── │   3000   │
 └──────────┘       └──────────┘        └──────────┘
      a                 b = &a              c = &b      *c == b, **c == a == 10
```

In the diagram in the notes (p.18) both cells are **empty** (P2-02), and under the heading "pointer to pointer"
only one level is drawn (P2-N2). Above is the complete version.

### Pointer arithmetic

The notes list `++, --, +, -` (P2-01). Also allowed are `+=`, `-=`, **the difference of two pointers**
within the same array (type `ptrdiff_t`) and the comparisons `<, >, <=, >=` (C11 6.5.6, 6.5.8).
`p + k` advances by `k * sizeof *p` **bytes**, not by k.

**Bounds (C11 6.5.6p8):** you may form a pointer to the elements of an array and to the position *one past the end*.
Even *computing* `p + 16k` further past the end is UB, before any dereference. That is why in
`bench/bench_search.c` the prefetch address is clamped to n (and the cost of going out of bounds is shown separately:
38.1 ns vs 6.05 ns per search at n = 1024).

### Printing addresses — UB in the notes

```c
printf("address of a = %u \n", &a);   // the notes, p.19–20
printf("address of a = %d \n", b);
```

Passing an `int *` for `%u`/`%d` is **undefined behavior** (C11 7.21.6.1p9). On a 64-bit machine
only the low 32 bits of the address are printed, which is why a "negative address" appears in the notes:
3010494292 − 2³² = −1284473004 — the same pointer read as a signed 32-bit int (P2-04).
Correct:

```c
printf("address of a = %p\n", (void *)&a);
```

## Structures

### The code in the notes doesn't compile

| Line in the notes | Problem | ID |
|---|---|---|
| `void main()` | clang: `error: 'main' must return 'int'` | P2-08 |
| `#include <conio.h>`, `getch()` | Turbo C/DOS, not ISO C | P2-09 |
| `int mobile;` | a 10-digit number > INT_MAX = 2 147 483 647; a phone number is a string, not a number | P2-10 |
| `printf (%d %f %d", &e3.id, ...)` | missing `"`, and it should be `scanf` — e3 is never read | P2-11 |

Size check (`make experiments` → `ex_struct_padding`):

```
sizeof(struct employee_notes) = 28 (id@0 name@4 mobile@24)
sizeof(bad_order)  = 32  {char, double, char, int, char}
sizeof(good_order) = 16  {double, int, char, char, char}
```

**Field order changes the size by a factor of two.** The compiler cannot reorder fields (C11 6.7.2.1p15:
members are laid out in declaration order), but it can insert padding for alignment.
Rule: declare fields from the largest alignment to the smallest.

### Experiment: Array of Structs vs Struct of Arrays

If only one field is needed (sum of salaries), AoS reads the whole record from memory, while SoA reads only the needed field:

```
Sum of salaries over 1048576 records (best of 7):
  AoS (64 B/record, reading 64 MB): 0.79 ns/record
  SoA ( 8 B/record, reading 8 MB): 0.26 ns/record
  SoA speedup: 3.0x
```

Measurement pitfall: with **one** accumulator `s += x` the difference was only 1.3×. The loop
was bound by FP-add latency (each iteration waits for the previous one), not by memory. Only
4 independent accumulators revealed the real difference. Honest caveat: 3.0× (3.0–3.3× across runs) is not just "8 times fewer bytes".
The 8 MB of SoA fit in L2 (16 MB) and are read from cache from the second repetition on, while the 64 MB of AoS come from DRAM every time.
The measured speedup = fewer bytes + caching; the two contributions are not separated here.

## Check yourself

1. Why is `sizeof(struct {char c; int i;})` most likely 8 and not 5? What does it depend on?
2. Why is `int *p = arr + 10;` valid for `int arr[10]`, while `arr + 11` is UB even without `*`?
