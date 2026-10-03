# DSA — handwritten data structures & algorithms notes, verified with C code and measurements

A repository for studying the **"DSA Handwritten Notes"** (109 pages, Topper World) on a
*"don't trust — verify"* basis. Every topic comes with:

- **explanations** in English, with corrections ([`docs/`](docs/));
- **working C11 code** — 18 sorts, 8 searches, lists, skip list, stack/queues/deque, BST/AVL/heap, graphs ([`src/`](src/));
- **tests** under AddressSanitizer + UBSan: 227 436 checks, including exact operation-count formulas ([`tests/`](tests/));
- **benchmarks** with real metrics from this machine, and **charts** built only from CSV ([`bench/`](bench/), [`results/`](results/), [`charts/`](charts/));
- **experiments** that reproduce the notes' errors "before / after", and for UB — with a real sanitizer report ([`experiments/`](experiments/));
- **a full errata register** for the notes, where every item was checked against the page image ([`errata/ERRATA-full.md`](errata/ERRATA-full.md));
- **a verbatim transcript** of the manuscript with the original errors deliberately preserved ([`transcript/`](transcript/)).

## Key finding: the notes contain 286 issues

![errata](charts/errata_by_chapter.png)

**119 clear-cut errors + 167 imprecisions.** Verification rejected 9 more candidates: mostly these
were misread words in the manuscript.
Method: 6 agents independently transcribed ~18 pages each. Then 3 independent verifiers re-checked
**every** item against the page image (130–400 dpi). C code was compiled with `clang -std=c11 -Wall -Wextra`
and run under ASan/UBSan, and BFS/DFS traces, trees, and address arithmetic were redone by hand.

**36 issues are compile errors (on 20 different pages)**, and clang rejects several more programs because of `void main`
(`error: 'main' must return 'int'`). Most of the notes' C programs do not build as written.

### The 12 most important errors

| # | Where | Error | Consequence | Evidence |
|---|---|---|---|---|
| 1 | p.34, list | `malloc(sizeof(struct node *))` | allocates 8 B instead of 16: **heap-buffer-overflow** | `ex_ub_malloc_sizeof_pointer` (ASan) |
| 2 | p.87, heap sort | `if (largest != 1)` instead of `!= i` | **infinite recursion** → SIGSEGV | `ex_heapify_bug` |
| 3 | p.83, "bucket sort" | `int bucket[max]`, indices 0..max; it is actually counting sort | **stack-buffer-overflow** | `ex_ub_counting_bucket` (ASan+UBSan) |
| 4 | p.107, stack | `isfull: top == MAXSIZE` | writes `stack[8]` into an 8-element array | `ex_ub_stack_isfull` (ASan) |
| 5 | p.16–17 | "Ω = best case, Θ = average, O = worst" | conceptual error: bounds ≠ cases | [02](docs/02-algorithms-asymptotics.md) |
| 6 | p.69, DFS | vertex is marked at push time | the order is **not DFS**: A D C B instead of A D B C | `ex_dfs_mark_on_push` |
| 7 | p.79–80, bubble | pseudocode does 1 pass; C code is exchange sort; "best O(n)" without a flag | does not sort / wrong algorithm | `ex_bubble_variants` |
| 8 | p.89, insertion | `temp <= a[j]` | **unstable**; Θ(n²) on equal keys (127 992 000 shifts at n=16000 vs 0) | `ex_insertion_le` |
| 9 | p.38–39, skip list | the `while` lacks `< key` and the advance step; relinking on the wrong levels | pseudocode does not work | [06](docs/06-skip-list.md) |
| 10 | p.25, arrays | `BA + size·(first − index)` | sign reversed; the notes' own example computes with the correct formula | [04](docs/04-arrays.md) |
| 11 | p.69, spanning tree | "edges = edges(graph) − 1" | correct is \|V\| − 1 | [09](docs/09-graphs.md) |
| 12 | p.74, 77 | `(beg + end) / 2`, `scanf("%d", item)` without `&` | int overflow (UB) / UB | `ex_binary_overflow` |

## Study path

| Step | Notes chapter (notebook pages) | Document | Code | Issues |
|---|---|---|---|---|
| 0 | — | [Measurement methodology](docs/00-methodology.md) | `bench/bench_util.h` | — |
| 1 | Intro, classification (3–8) | [01](docs/01-intro-classification.md) | — | 15 |
| 2 | Algorithms, asymptotics (8–17) | [02](docs/02-algorithms-asymptotics.md) | `bench/bench_ops.c` | 23 |
| 3 | Pointers, structures (18–22) | [03](docs/03-pointers-structures.md) | `experiments/ex_struct_padding.c` | 9 + 6 |
| 4 | Arrays (22–30) | [04](docs/04-arrays.md) | `bench/bench_ds.c` (traverse) | 20 |
| 5 | Linked lists (29–36) | [05](docs/05-linked-lists.md) | `src/list.c` | 11 |
| 6 | Skip list (36–40) | [06](docs/06-skip-list.md) | `src/skiplist.c` | 18 |
| 7 | Stack and queue (41–47) | [07](docs/07-stack-queue.md) | `src/stack_queue.c` | 8 + 6 |
| 8 | Trees, BST, AVL, B/B+, heap (48–62) | [08](docs/08-trees.md) | `src/tree.c` | 23 + 11 |
| 9 | Graphs, BFS/DFS, MST (62–71) | [09](docs/09-graphs.md) | `src/graph.c` | 10 + 18 |
| 10 | Searching (71–78) | [10](docs/10-searching.md) | `src/search.c` | 11 |
| 11 | Sorting (78–92) | [11](docs/11-sorting.md) | `src/sort.c` | 41 |
| 12 | 50 interview questions (93–103) | [12](docs/12-interview-qa.md) | — | 27 |
| 13 | Coding problems (104–108) | [13](docs/13-coding-questions.md) | — | 29 |

How to work through each chapter: read the notes' pages (`transcript/`) → the `docs/NN` document →
the code in `src/` → run the corresponding experiment and look at the chart → hide the errata for the chapter
and try to find every error yourself before reading the explanation.

## What the measurements showed (Apple M4 Pro)

| Experiment | Result | Chart |
|---|---|---|
| Array traversal vs shuffled list, n = 2²⁴ | 0.044 vs 111.3 ns/element — **~2 500× apart**, although both are Θ(n) | [traverse](charts/traverse.png) |
| BST on sorted input vs AVL, n = 32 768 | height 32 768 vs 16; search **1 503×** slower | [bst_avl](charts/bst_avl.png) |
| Binary search vs Eytzinger layout, 4 MB | 71.8 vs 17.5 ns (**4.1×**) — same O(log n), only the data layout changes | [search_time](charts/search_time.png) |
| Interpolation search: uniform random vs skewed keys, n = 2²⁰ | 11.6 vs 1 746 comparisons on average (binary: 36.9) | [search_ops](charts/search_ops.png) |
| Quicksort Lomuto (pivot = last) on sorted input, n = 16 384 | 134 209 536 = n(n−1)/2 comparisons | [sort_ops](charts/sort_ops.png) |
| Dynamic array growth +1024 vs ×2 (with memcpy), n = 2²⁰ | 28.0 vs 0.31 ns/push; with `realloc` on macOS there is **no** difference: the block grows in place | [stack_growth](charts/stack_growth.png) |
| Skip list, memory at p = 1/2, 1/4, 1/8 | 2.001 / 1.334 / 1.144 pointers per node = 1/(1−p) | [skiplist](charts/skiplist.png) |
| BFS: matrix vs CSR, V = 16 384, E = 4V | 62.8 ms vs 0.30 ms; 256 MB vs 640 KB. On a complete graph (V = 4096) — 3.9 vs 4.3 ms: the matrix catches up | [graph_bfs](charts/graph_bfs.png) |
| Heap construction: Floyd vs n × push, 2²⁴ | 2.56 vs 7.09 ns/element | [heap_build](charts/heap_build.png) |
| Insertion vs merge at small n | insertion is faster up to n = 128, slower from n = 256 | `ex_small_n_crossover` |

The [methodology](docs/00-methodology.md) describes, with corrections:
- 5 measurement pitfalls I ran into myself: 1 µs macOS timer resolution, prefetch past the end of the array,
  FP-add latency, in-place `realloc`, bubble sort codegen;
- 6 of my predictions that the measurements refuted;
- 6 methodology defects and 3 code bugs found by an independent code review (the data was regenerated afterwards).

This is part of the learning material: this is what checking claims looks like in practice.

## Running

```sh
make test                 # unit tests under ASan + UBSan
make experiments          # all demonstrations of the notes' errors
make bench                # ~3–5 min → results/*.csv
python3 -m venv .venv && .venv/bin/pip install matplotlib numpy
make plots                # charts/*.png from results/*.csv
```

Only a C11 compiler (clang/gcc) and make are required; Python is needed only for charts.
`experiments/ex_ub_*` are built with `-fsanitize=address,undefined`: the buggy code runs in a child
process, and you see the real sanitizer report.

## Layout

```
include/      headers (API with comments: complexity, conventions, edge cases)
src/          implementations
tests/        unit tests (minitest.h — 30 lines, no dependencies)
bench/        benchmarks → results/*.csv
experiments/  reproductions of the notes' errors and of measurement pitfalls
scripts/      plot.py — charts from CSV only
results/      raw measurements (CSV)
charts/       charts (PNG)
docs/         per-chapter explanations
errata/       ERRATA-full.md (286 + 9 rejected), errata.csv
transcript/   verbatim transcript of the manuscript (in English)
```

## Limitations (honestly)

- Absolute times are for a single machine (Apple M4 Pro, macOS 27, clang 21). The shape of the curves transfers; the numbers do not.
- Hardware counters (cache/branch misses) are unavailable on macOS without root. Cache-based explanations rely on
  where the curves bend relative to the L1/L2 sizes, not on direct miss measurements.
- The manuscript was transcribed from images. Places where the handwriting is ambiguous are marked in the errata
  (e.g., P6-N4 — "low confidence").
