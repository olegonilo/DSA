# 12. 50 interview questions: what the notes get wrong

Notes: notebook p.93–103 (PDF p.99–109; the page order in the PDF differs from the notebook here).
Below are only the answers that contain an error or imprecision. Answers not listed here were judged
correct by the verifiers. Full wording — section 16 in [`errata/ERRATA-full.md`](../errata/ERRATA-full.md).

## Unambiguous errors (fix them before an interview)

| Q | The notes | Correct answer | ID |
|---|---|---|---|
| Q9 | "methods available in **storing** sequential files" | The question is about **sorting** sequential files (external sorting): straight merging, natural merging, polyphase sort, distribution of initial runs | P6-35 |
| Q12 | among the factors of sorting efficiency — "object oriented analysis and design" | Unrelated to sorting. Factors: number of comparisons and moves, extra memory, stability, behavior on partially sorted data, memory locality | P6-38 |
| Q22 | code to check whether a list is circular: `while(pointer1){… pointer2 = pointer2->next …}` | The code dereferences NULL on a non-circular list and prints "circular" for a 1-node list. Correct: Floyd's algorithm with the condition `fast && fast->next` (`sll_has_cycle`) | P6-43 |
| Q25 | open addressing uses an "overflow block" | Open addressing = **probing** within the table: linear, quadratic, double hashing (CLRS §11.4). Overflow areas are a different scheme | P6-45 |
| Q39 | `signed`/`unsigned` — "type qualifiers" | These are **type specifiers** (C11 6.7.2). Qualifiers: `const`, `volatile`, `restrict`, `_Atomic` (6.7.3) | P6-52 |

## Imprecisions interviewers "catch"

| Q | What's wrong | ID |
|---|---|---|
| Q3 | "RDBMS = array of structures" — a memorized answer at the logical level. Real DBMSs use B+-trees and hash indexes | P6-32 |
| Q4 | "for a list with heterogeneous data an ordinary pointer is impossible, a void pointer is needed" — the links can stay typed (`struct node *`); `void *` or a `union` is only needed for the **data** | P6-33 |
| Q5 | "a priority queue needs **two** queues" — folklore; the standard implementation is a heap, with no queues at all | P6-34 |
| Q10 | "by storage method a linked list is non-linear" — linearity is a logical property; the notes' own Q43 calls a list linear | P6-36 |
| Q13 | "linear search: O(n) comparisons in every case" — best case Θ(1) (found first), average/worst Θ(n) | P6-39 |
| Q15 | "postfix notation needs no parentheses" — only if every operator has a fixed arity (unary minus breaks this) | P6-40 |
| Q19 | AVL: "pivotal value or height factor" — correct term is **balance factor**; rebalancing at ±2 | P6-41 |
| Q20 | why a B+-tree for indexes: the key points are missing — high fan-out (lower height → fewer disk reads) and linked leaves (range queries) | P6-42 |
| Q23 | "node class" is a definition from C++ class design (Stroustrup, TC++PL 3rd ed. §25.4), not the DSA notion of a node (data + links) | P6-44 |
| Q27, Q28 | duplicates of Q7 and Q16 | P6-46 |
| Q29 | malloc vs calloc: it is not mentioned that `malloc` **does not initialize** memory, while `calloc(nmemb, size)` zeroes it (common implementations — glibc, macOS libmalloc — also return NULL on nmemb·size overflow) | P6-47 |
| Q31 | `front = (front + 1) % size` is presented as a general rule; it is only the advance of front on dequeue in a circular queue | P6-49 |
| Q35 | "isEmpty() checks whether there is at least one element" — the opposite: it returns true when there are **no** elements | P6-50 |
| Q37 | "push is the direction" — push is an operation, not a direction | P6-51 |
| Q40 | a pointer is called a data structure | P6-53 |
| Q42 | stack and queue are called dynamic — this depends on the implementation (the stack from coding question 3 is static) | P6-54 |
| Q44 | general graphs are "hierarchical" — no, only trees are hierarchical | P6-55 |
| Q45 | the list of list types is missing the doubly linked circular list | P6-56 |
| Q46 | "insertion of a list", "traversal of a node" — the other way round: insertion of a *node*, traversal of a *list*; search is missing | P6-57 |
| Q50 | sequential search "until a match" — or until the end of the array (unsuccessful search) | P6-58 |

## How to practice

For every row above the repository has code that lets you **demonstrate** the answer rather than just state it:
Q22 → `sll_has_cycle`; Q5 → `minheap`; Q13 → `results/search_ops.csv`; Q15 → `infix_to_postfix`/`eval_postfix`;
Q19 → `avl_insert` + `test_avl` (4 rotation cases); Q29 → `stack_reserve` (correct `realloc` into a temporary variable).
