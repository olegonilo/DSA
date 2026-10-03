# 07. Stack and queue

The notes: notebook p.41–47 (PDF p.42–48) + coding question 3 (notebook p.107–108).
Code: [`src/stack_queue.c`](../src/stack_queue.c).

## Stack (LIFO)

| Operation | Complexity | Note |
|---|---|---|
| push | Θ(1) amortized | with ×2 growth; with +K growth — Θ(n/K) amortized, see the experiment |
| pop / peek | Θ(1) | `peek` returns the **top** element; the notes: "element at a given position" (P3-19) — that is `peep(i)` |
| isEmpty / isFull | Θ(1) | |

### Errors in the notes' push/pop

Pseudocode on p.43 (P3-24, P3-25):

```
if top = n then stack full        ← after the message, execution CONTINUES
top = top + 1
stack(top) := item                ← out-of-bounds write
```

The bounds check must **terminate** the operation. Moreover, the test `top = n` contradicts the convention "empty ⇔ top = −1"
on the same page: for indices 0..n−1, full ⇔ `top = n − 1`.

The same off-by-one appears in the C code of coding question 3 (P6-25):

```c
int MAXSIZE = 8; int stack[8]; int top = -1;
int isfull() { if (top == MAXSIZE) return 1; else return 0; }   // should be MAXSIZE - 1
```

`make experiments` → `ex_ub_stack_isfull` (9 pushes onto a stack of 8):

```
1) isfull: top == MAXSIZE (the notes), 9 pushes into an 8-element stack
      | runtime error: index 8 out of bounds for type 'int[8]'
      | ERROR: AddressSanitizer: global-buffer-overflow
      | 0x… is located 0 bytes after global variable 'stack_' … of size 32
   [the notes] child process killed by signal 6 (Abort trap: 6)
2) isfull: top == MAXSIZE - 1 (fixed)
      could not insert data, stack is full
      top after 9 pushes = 7
```

The notes themselves push only 6 times, so the bug "sleeps" and the error is not visible. Also there: `pop()` in the
"stack is empty" branch returns no value (P6-27, UB if the result is used), and the last `printf`
does not compile — it is missing `?` and `:` (P6-31).

### Experiment: dynamic array growth strategy

![stack_growth](../charts/stack_growth.png)

| Strategy (explicit `malloc+memcpy`) | n = 2¹⁶ | n = 2²⁰ | elements copied at n = 2²⁰ |
|---|---|---|---|
| ×2 | 0.31 ns/push | 0.31 ns/push | 1 048 568 (< n) |
| +1024 | 1.87 ns/push | **28.0 ns/push** | 536 346 624 (≈ n²/2048) |

Theory confirmed: with ×2 growth the total copying is < n (geometric series), so push is Θ(1)
amortized. With +K growth Θ(n²/K) elements are copied, so the time per push grows linearly with n.

**But with `realloc` on macOS** (right panel) both strategies give a flat line. The cause was measured, not guessed
(`ex_realloc_inplace`): large blocks are extended *in place*, without copying:

```
   size  realloc    realloc time   malloc+memcpy of the same size
  64 MB  in place      12.7 µs                   9381.2 µs
 256 MB  in place       6.1 µs                  24835.1 µs
```

Conclusion: "+K gives Θ(n²)" is a property of the *memory model*, not a law of nature. Doubling is always
correct. Relying on the behavior of a particular allocator's `realloc` is not.

### Stack applications (implemented and tested)

- `balanced_brackets("{[()()]}")` → 1; `"([)]"` → 0.
- `infix_to_postfix` (Dijkstra's shunting-yard) with correct associativity: `a-b-c` → `ab-c-`
  (left-associative), `a^b^c` → `abc^^` (right-associative). Verified: `2^3^2` = 512, not 64.
- `eval_postfix("231*+9-")` = −4.

## Queue (FIFO)

### A linear queue wastes space

```
cap = 3: enqueue 1,2,3 → [1 2 3] front=0 rear=3
dequeue ×2            → [· · 3] front=2 rear=3
enqueue 4             → OVERFLOW, although 2 cells are free      (test: lq_enqueue → -1)
```

The notes name "insertion only at rear" as the drawback of a linear queue (P3-28). But that is the *definition* of a queue.
The real drawback is the false overflow shown above.

### Circular queue

The index must **wrap around**: `rear = (rear + 1) % cap` (the notes: "simply incrementing rear", P3-29).
The `cqueue` implementation stores `count` instead of "leaving one cell empty", so all `cap` cells are usable
(test: a queue of 3 accepts exactly 3).

### Errors in the queue diagrams (p.45–47)

- Rear is shown under 20 instead of 30 (P3-27).
- "front increases from −1 to 0" after deletion (P3-31). In fact it goes from 0 to 1, as the notes' own drawing shows.
- "Go to step [END OF IF]" — the step number is missing (P3-32).

### Priority queue

The efficient implementation is a binary heap: `heap_push`/`heap_pop` in Θ(log n) (see [08-trees](08-trees.md#binary-heap)).
Interview answer Q5 "two queues are needed" (P6-34) is folklore: a heap needs no queues at all.
