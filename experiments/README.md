# Experiments

`make experiments` builds and runs all of them. Each file starts with a comment: what exactly is wrong in the notes,
with the page number, and how to fix it.

| File | What it reproduces | Build |
|---|---|---|
| `ex_ub_malloc_sizeof_pointer.c` | `malloc(sizeof(struct node *))` (notebook p.34) → heap-buffer-overflow | ASan+UBSan |
| `ex_ub_stack_isfull.c` | `isfull: top == MAXSIZE` (p.107) → global-buffer-overflow | ASan+UBSan |
| `ex_ub_counting_bucket.c` | `int bucket[max]` with indices 0..max (p.83) → stack-buffer-overflow | ASan+UBSan |
| `ex_heapify_bug.c` | `largest != 1` (p.87) → infinite recursion (depth capped by a counter) | -O0 |
| `ex_bubble_variants.c` | 1-pass pseudocode; a "bubble" that is actually exchange sort; best case | -O0 |
| `ex_insertion_le.c` | `temp <= a[j]` → instability and Θ(n²) on equal keys | -O0 |
| `ex_dfs_mark_on_push.c` | DFS that marks on push yields a non-DFS order | -O0 |
| `ex_binary_overflow.c` | `(beg + end) / 2` → negative mid | -O0 |
| `ex_struct_padding.c` | padding, `int mobile` for a 10-digit number, AoS vs SoA | -O2 |
| `ex_realloc_inplace.c` | why `realloc` hides the Θ(n²) of linear growth | -O2 |
| `ex_small_n_crossover.c` | up to which n Θ(n²) insertion beats Θ(n log n) | -O2 |

`ex_ub_*` run the buggy code in a child process (`fork`) and print only the sanitizer's diagnostic
lines. That is why `make experiments` succeeds even though the child processes crash — this is intended.
