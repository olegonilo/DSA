# 05. Linked lists

The notes: notebook p.29–36 (PDF p.30–37). Code: [`src/list.c`](../src/list.c), tests: `tests/test_ds.c`.

## Kinds

```
Singly linked:          head → [10|•] → [20|•] → [30|∅]
Doubly linked:          head → [∅|10|•] ⇄ [•|20|•] ⇄ [•|30|∅]
Circular:               head → [10|•] → [20|•] → [30|•] ─┐
                                 ▲───────────────────────┘
Doubly linked circular: prev of the first = last, next of the last = first; no NULL anywhere (P2-38)
```

## Critical error: `malloc(sizeof(struct node *))`

The notes, p.34 (P2-40):

```c
ptr = (struct node *) malloc(sizeof(struct node *)     // as in the notes (also missing ')' and ';')
```

`sizeof(struct node *)` is the size of a **pointer** (8 B), not of a node (16 B: 4 int + 4 padding + 8 pointer).
Writing `ptr->next = …` writes past the end of the block. This is a real heap overflow; `make experiments` → `ex_ub_malloc_sizeof_pointer`:

```
sizeof(struct node *) = 8, sizeof(struct node) = 16, offsetof(next) = 8
1) code from the notes: malloc(sizeof(struct node *))
      | ERROR: AddressSanitizer: heap-buffer-overflow on address 0x602000000998
      | WRITE of size 8 at 0x602000000998 thread T0
      | 0x602000000998 is located 0 bytes after 8-byte region [0x602000000990,0x602000000998)
   [the notes] child process killed by signal 6 (Abort trap: 6)
2) fixed: malloc(sizeof *ptr)
   [fixed] child process exited with code 0
```

**The idiom that rules out this error:** `ptr = malloc(sizeof *ptr);` — the type is taken from the variable itself.
Plus a `NULL` check (the notes never have one).

Also: the 4 `struct node {...}` definitions on p.31–33 lack a `;` after `}` (P2-35) — clang: `expected ';' after struct`.

## Complexity — corrections to the notes' table

The notes (p.34) give O(1) for insertion and deletion in a singly linked list in all cases (P2-39).

| Operation | Singly linked (head) | Singly linked (head+tail) | Doubly linked |
|---|---|---|---|
| insert at front | Θ(1) | Θ(1) | Θ(1) |
| insert at end | **Θ(n)** | Θ(1) (`sll_push_back`) | Θ(1) |
| insert at position k | Θ(k) — finding the spot | Θ(k) | Θ(k) |
| delete first | Θ(1) | Θ(1) | Θ(1) |
| delete last | Θ(n) | **Θ(n)** — needs the second-to-last | Θ(1) (`dll_pop_back`) |
| delete a known node | Θ(n) — needs the predecessor | Θ(n) | **Θ(1)** (`dll_unlink`) |
| search | Θ(n) | Θ(n) | Θ(n) |

O(1) holds only at the head, or when the predecessor is **already known**.

## Deletion cases

The notes: "The list can either be empty or full" (P2-42) — **a linked list has no "full" state**.
The correct cases: (1) empty, (2) one node — both `head` and `tail` are updated,
(3) more than one node. `sll_remove_value` handles them with a single code path via a pointer to a pointer:

```c
sll_node **pp = &l->head, *prev = NULL;
while (*pp && (*pp)->data != v) { prev = *pp; pp = &(*pp)->next; }
if (!*pp) return -1;
sll_node *dead = *pp;
*pp = dead->next;                 // works for both the head and the middle
if (l->tail == dead) l->tail = prev;
free(dead);
```

## Classic techniques (implemented and tested)

| Technique | Function | Idea |
|---|---|---|
| Reversal in O(1) memory | `sll_reverse` | three pointers prev/cur/next |
| Cycle detection (Floyd) | `sll_has_cycle` | slow +1, fast +2; they meet if there is a cycle |
| Middle in 1 pass | `sll_middle` | fast reaches the end when slow is in the middle |
| Josephus problem | `josephus_cll` | circular list; verified J(41,3) = 31, J(7,3) = 4 |

Interview question Q22 in the notes ("how to check that a list is circular") contains code that **dereferences NULL**
on a non-circular list and wrongly says "circular" for a single-node list (P6-43). The correct answer:
`sll_has_cycle` (condition `fast && fast->next`).

## Why lists are slow in practice

See [04-arrays](04-arrays.md#experiment-on-array-traversal-vs-linked-list-traversal): traversing a shuffled
list of 16M nodes costs 111.3 ns/node vs 0.044 ns/element for an array.
