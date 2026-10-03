# Errata for the handwritten DSA notes (C)

This file lists the errors and imprecisions found in the handwritten notes on data structures and algorithms. A verbatim transcript of the notes (in English, with the original errors deliberately preserved) is in [`../transcript/`](../transcript/).

## Method

1. **Transcription.** Six transcriber agents each independently transcribed one part of the PDF (6 parts, PDF pages 1–109) and compiled initial lists of candidate errors.
2. **Verification.** Three independent verifier agents re-checked *every* candidate against the page images, rendering doubtful spots at 130 to 400 dpi. Every claim about C was reproduced by compiling with `clang -std=c11 -Wall -Wextra` and, where appropriate, running with `-fsanitize=address,undefined` (ASan/UBSan). Computations (BFS/DFS, spanning trees, address arithmetic, algorithm traces) were redone by hand.
3. The verifiers also added previously missed items (verdict NEW) and rejected false candidates (see the "Rejected candidates" section).

The notebook page number is the number in the top-right corner of the page (from PDF p. 4 through PDF p. 93 it equals PDF − 1). In the PDF 94–109 range the PDF order does not match the notebook order: PDF pp. 94–98 = notebook pp. 104–108 (coding problems), PDF pp. 99–109 = notebook pp. 93–103 (interview questions). That is why chapter 16 comes before chapter 17 even though its PDF pages are later.

Quotes are given verbatim in the original language (English); code is in backticks. Identifiers, code, and references to standards and textbooks (C11 6.5p5, CLRS §3.1, Knuth 6.2.3, etc.) are kept as is.

### Legend

| Label | Meaning |
|---|---|
| **ERROR** | A clearly false statement, code that does not compile, or code with undefined behavior. |
| **IMPRECISE** | An imprecise, incomplete, or misleading statement. |
| CONFIRMED | The verifier confirmed the item as originally stated. |
| CWC | Confirmed with correction: the item is essentially right, but the verifier corrected its reasoning, scope, or severity. |
| NEW | An item missed by the transcribers and added by a verifier. |
| REJECTED | The candidate turned out not to be an error; such items are listed in a separate section and excluded from the summary. |

"(downgraded)" next to the severity means the verifier lowered it from ERROR to IMPRECISE.

Some items from pages on chapter boundaries are assigned to a chapter by topic: P1-12…P1-14 (operations on data structures, PDF pp. 9–10) to chapter 01; P3-01 (list traversal, PDF p. 37) to chapter 06; P4-07 and P4-08 (BST, PDF pp. 56–57) to chapter 11; P4-29 and P4-30 (graph traversals, PDF p. 68) to chapter 13; P5-N1 (bubble sort, PDF p. 79) to chapter 15.

## Contents

- [01. Intro & DS classification](#01) (PDF 1–9)
- [02. Algorithms & asymptotic analysis](#02) (PDF 9–18)
- [03. Pointers](#03) (PDF 19–21)
- [04. Structures](#04) (PDF 22–23)
- [05. Arrays](#05) (PDF 24–31)
- [06. Linked lists](#06) (PDF 31–37)
- [07. Skip list](#07) (PDF 37–41)
- [08. Stack](#08) (PDF 42–44)
- [09. Queue](#09) (PDF 45–48)
- [10. Trees](#10) (PDF 49–58)
- [11. Tree types: BST/AVL/B/B+](#11) (PDF 58–63)
- [12. Graphs](#12) (PDF 63–69)
- [13. Graph traversals / spanning trees](#13) (PDF 69–72)
- [14. Searching](#14) (PDF 72–79)
- [15. Sorting](#15) (PDF 79–93)
- [16. Interview Q1–Q50](#16) (PDF 99–109, notebook 93–103)
- [17. Coding problems](#17) (PDF 94–98, notebook 104–108)
- [Rejected candidates (not errors)](#rejected)
- [Summary](#summary)

<a id="01"></a>

## 01. Intro & DS classification (PDF 1–9)

| ID | PDF/nb. page | Quote | Category | Severity | Verdict | Why it is wrong | Correct version |
|---|---|---|---|---|---|---|---|
| P1-01 | 4 / 3 | "It is set of algorithms that we can use in any programming language to structure data in memory" | terminology | **ERROR** | CONFIRMED | The quote is on the page. A data structure is a way of organizing data together with the operations on it, not a set of algorithms. | "A language-independent way of organizing data in memory together with the operations on it." |
| P1-02 | 4 / 3 | "primitive data structure" → int, char, float, double, pointer | terminology | **IMPRECISE** | CONFIRMED | These are primitive *data types*. The usage is common in syllabi but is not standard terminology. | "Primitive data types …; non-primitive data structures …" |
| P1-03 | 4–5 / 3–4 | "one element is connected to only one another element in a linear form" | factual | **IMPRECISE** | CONFIRMED | Interior elements have one predecessor and one successor, i.e. two neighbors. | "Every element except the ends has exactly one predecessor and one successor." |
| P1-04 | 5 / 4 | "When one element is connected to the 'n' number of elements" | factual | **IMPRECISE** | CONFIRMED | Being connected to n elements is not the criterion for non-linearity. A doubly linked list node has 2 links, yet the list is still linear. | "An element may have several predecessors/successors, so the elements do not form a single sequence." |
| P1-05 | 5 / 4 | "elements are arranged in a random manner" | factual | **IMPRECISE (downgraded)** | CWC | The quote is accurate. "Random" is used loosely to mean "not sequential", so it is misleading rather than false. The structure is hierarchical (tree) or defined by an edge set (graph). Severity downgraded from ERROR. | "Elements are organized hierarchically (trees) or as a network (graphs)." |
| P1-06 | 5 / 4 | "all these algorithms are knowns as Abstract Data Types" | terminology | **ERROR** | CONFIRMED | An ADT is a type defined by its values and operations, not a collection of algorithms. Contradicts p. 6 ("ADT tells what"). | "An ADT specifies a set of values and operations independently of the implementation." |
| P1-07 | 6 / 5 | "60 employees in class, then there will be 20 records" | inconsistency | **ERROR** | CONFIRMED | Checked at 400 dpi: the digits are "60" and "20". The source (javatpoint) has the same text. One record per employee means 60 records. | "60 employees → 60 records" |
| P1-08 | 6 / 5 | "An entity represents class of certain objects" | terminology | **IMPRECISE** | CONFIRMED | In ER modeling an entity is a single object. A class of objects is an entity set/type. | "An entity is an object; an entity set is a class of similar entities." |
| P1-09 | 7 / 6 | "Data Structure :- consider an inventory size of 106 items" | meaningful typo | **ERROR** | CONFIRMED | The page has the heading "Data Structure". The item describes the search problem; the source's heading is "Data Search". | "Data Search :- …" |
| P1-10 | 7 / 6 | "require data can be searched instantly" | factual | **IMPRECISE** | CONFIRMED | Overstatement. Typical cost is O(log n), or expected O(1) for hashing (Θ(n) worst case). | "…only a small fraction of the data needs to be examined" |
| P1-N1 | 7 / 6 | "inventory size of 106 items … transverse 106 items" | meaningful typo | **ERROR** | NEW | The source (javatpoint) has 10⁶ (a million); the superscript was lost. "106 items" defeats the scalability argument: scanning 106 items is trivial. | "10⁶ (a million) items" |
| P1-11 | 8 / 7 | Static: Array; Dynamic: Linked list, Stack, Queue | factual | **IMPRECISE** | CONFIRMED | Stacks and queues can be implemented with either an array or a linked list. | State that both implementations exist. |
| P1-12 | 9 / 8 | "If the size of data structure is n then we can only insert n-1 data elements" | factual | **ERROR** | CONFIRMED | A structure of capacity n holds n elements. Inserting into a full structure is an overflow. | "at most n elements; trying to insert more → overflow" |
| P1-13 | 9–10 / 8–9 | "There are two algorithms to perform searching, linear search and Binary search" | factual | **IMPRECISE** | CONFIRMED | There are many other search methods (hashing, interpolation search, BST search, etc.). | "The two basic methods for searching an array are …" |
| P1-14 | 10 / 9 | merging: "clubbed or joined to produce third list … (M+N)" | terminology | **IMPRECISE** | CONFIRMED | This describes concatenation. In DSA, merging usually means combining two sorted lists into one sorted list (CLRS §2.3.1). | "Merge two sorted lists into one sorted list of size M+N." |

<a id="02"></a>

## 02. Algorithms & asymptotic analysis (PDF 9–18)

| ID | PDF/nb. page | Quote | Category | Severity | Verdict | Why it is wrong | Correct version |
|---|---|---|---|---|---|---|---|
| P1-15 | 10 / 9 | "An algorithm has some input values. We can pass 0 or some input value" | inconsistency | **IMPRECISE** | CONFIRMED | Minor internal contradiction ("has some input values" vs "0 or some"). Knuth: zero or more inputs. | "zero or more input values" |
| P1-16 | 11 / 10 | "finiteness :- … means limited number of instructions" | factual | **ERROR** | CONFIRMED | Knuth's finiteness means the algorithm terminates after a finite number of steps. A short program can run forever. | "the algorithm must terminate after a finite number of steps" |
| P1-17 | 11 / 10 | "Effectiveness :- An algorithm should have finite as each instruction … affects the overall process" | factual | **ERROR** | CONFIRMED | Incoherent wording. Effectiveness means each operation is basic enough to be carried out exactly in finite time. | "Each step must be basic enough to be carried out exactly in finite time." |
| P1-18 | 11 / 10 | optimizing: "…take out the best solution is known then it will terminate if the best solution is known" | inconsistency | **IMPRECISE** | CONFIRMED | Mixes the optimizing approach with early termination, which is characteristic of "sacrificing". | "Optimizing: enumerate all solutions and return the best (or stop early if the optimal value is known in advance). Sacrificing: stop as soon as a good-enough/best solution is found." |
| P1-19 | 12 / 11 | "This breaks down the algorithm to solve the problem in different methods" | factual | **ERROR** | CONFIRMED | Divide and conquer splits the *problem* into subproblems, solves them recursively, and combines the results. | "divide / conquer recursively / combine" |
| P1-20 | 12 / 11 | "makes an optimal choice on each iteration" | terminology | **IMPRECISE** | CONFIRMED | The word "locally" is missing. | "locally optimal choice" |
| P1-21 | 12 / 11 | "there are very rare cases in which it provides the optimal solution" | factual | **ERROR** | CONFIRMED | Greedy algorithms are optimal for many classic problems: Huffman coding, MST, activity selection, Dijkstra, fractional knapsack. | "optimal when the greedy-choice property and optimal substructure hold" |
| P1-22 | 13 / 12 | "The time complexity of an algorithm is denoted by the big O notation" | terminology | **IMPRECISE** | CONFIRMED | O, Ω, and Θ are all used. O is only an upper bound. | "Time complexity is usually expressed with asymptotic notation (O — upper bound, Ω — lower, Θ — tight)." |
| P1-23 | 14 / 13 | "// suppose we have to calculate the sum of n numbers" vs `sum = sum + i` | inconsistency | **IMPRECISE** | CONFIRMED | The loop computes 1+…+n, not the sum of n arbitrary input numbers. Minor. | "sum of the first n natural numbers" |
| P1-24 | 14 / 13 | "time complexity of the loop statement will be atleast n" | complexity | **IMPRECISE** | CONFIRMED | True but not tight: the loop is Θ(n). | "Θ(n)" |
| P1-N2 | 14 / 13 | "Space complexity = Auxiliary space + Input size." | terminology | **IMPRECISE** | NEW | "Input size" (n) is a count, not an amount of memory; what is meant is the memory occupied by the input. Many textbooks (and most interviews) count only auxiliary space, so the formula should state its convention explicitly. | "Space complexity = auxiliary space + space occupied by the input" |
| P1-25 | 15 / 14 | "The comparison operator decides the new order of the elements" | factual | **IMPRECISE** | CONFIRMED | True only for comparison sorts. Counting, radix, and bucket sort do not use pairwise comparisons. | "In comparison sorts the comparison operator determines the order; non-comparison sorts (counting, radix, bucket) use key values directly." |
| P1-26 | 16 / 15 | "Average case :- It takes average time for the program execution" | factual | **IMPRECISE** | CONFIRMED | Circular definition. | "expected running time under a given input distribution" |
| P1-27 | 16 / 15 | "ensures that function never grows faster than the upper bound" + curve labelled g(n) | factual / diagram | **IMPRECISE** | CONFIRMED | The page's own diagram shows f(n) above g(n) to the left of k, contradicting "never". The bound holds only for n ≥ n0 and up to a constant c. | label the curve c·g(n); "for all n ≥ n0" |
| P1-N3 | 16 / 15 | "Worst case :- It defines the input for which the algorithm takes a huge time." | factual | **IMPRECISE** | NEW | The worst case is the *maximum* running time over all inputs of size n, not a "huge time". The best-case definition likewise needs "over inputs of size n". | "T_worst(n) = maximum over all inputs of size n" |
| P1-28 | 17 / 16 | "exists constants c and n0 such that : f(n) ≤ c · g(n) for all n ≥ n0" | factual | **IMPRECISE** | CONFIRMED | The positivity condition (c > 0) is missing; p. 18 gives c>0 for Ω, so the definitions are inconsistent. | "∃ c>0, n0: 0 ≤ f(n) ≤ c·g(n) ∀ n ≥ n0" |
| P1-29 | 17, 18 / 16, 17 | "It basically describes best case scenario which is opposite to big o notation" | factual | **ERROR** | CONFIRMED | Confuses the kind of bound with case analysis. Ω is a lower bound and can be applied to any case. | "Ω gives an asymptotic lower bound: f(n) = Ω(g(n)) ⇔ ∃ c > 0, n0: f(n) ≥ c·g(n) ∀ n ≥ n0. This is independent of best/worst/average case." |
| P1-30 | 17 / 16 | Θ sandwich diagram and "c1.g(n) <= f(n) <= c2.g(n)" placed under "2). Omega Notation" | inconsistency / diagram | **ERROR** | CONFIRMED | Under the Ω heading the page has the caption "f(n) = Θ(g(n))". Section 2 is then repeated on p. 18. | Move the Θ material to section 3. |
| P1-31 | 17 / 16 | "where n is steps required to execute program" | factual | **ERROR** | CONFIRMED | n is the input size. f(n) is the number of steps. | "…where n is the input size and f(n) is the number of steps for an input of size n." |
| P1-32 | 17 / 16 | "satisfied only if when : c1.g(n) <= f(n) <= c2.g(n)" | factual | **IMPRECISE** | CONFIRMED | Missing the existential quantifiers on the constants and the "for all n ≥ n0" condition. | CLRS definition of Θ: ∃ c1, c2 > 0, n0: 0 ≤ c1·g(n) ≤ f(n) ≤ c2·g(n) ∀ n ≥ n0. |
| P1-33 | 18 / 17 | "f(n) is omega of g(n) or f(n) is on the order of g(n)" | terminology | **IMPRECISE (downgraded)** | CWC | Loose wording rather than a clear error. The same phrase is used for O on p. 16 and was rated IMPRECISE in the initial list (P1-28), so rating it ERROR here is inconsistent. Severity downgraded. | "f grows at least as fast as g" |
| P1-34 | 18 / 17 | "The theta notation mainly describes average case scenarios" | factual | **ERROR** | CONFIRMED | Θ is a tight (two-sided) bound; it does not depend on which case is analyzed. | "Θ = both O and Ω" |
| P1-35 | 18 / 17 | "Big theta is mainly used when the value of worst-case and best-case is same" | factual | **IMPRECISE** | CONFIRMED | Θ is also applied to each case separately. | "If the best- and worst-case times are both Θ(g(n)), the running time on all inputs is Θ(g(n)); Θ can also be applied to each case separately." |

<a id="03"></a>

## 03. Pointers (PDF 19–21)

| ID | PDF/nb. page | Quote | Category | Severity | Verdict | Why it is wrong | Correct version |
|---|---|---|---|---|---|---|---|
| P2-01 | 19 / 18 | "4 arithmatic operators that can be used in pointers : ++, --, +, -" | factual | **IMPRECISE** | CONFIRMED | `+=`, `-=`, pointer subtraction (within the same array), and relational comparisons are also allowed (C11 6.5.6, 6.5.8). | "Allowed: ptr ± integer (including ++, --, +=, -=), ptr − ptr (same array), and relational comparisons. Not allowed: ptr + ptr, ×, ÷." |
| P2-02 | 19 / 18 | diagram "b = &a → [ ] 3000 → [ ] 2000" | diagram | **IMPRECISE** | CONFIRMED | Both cells are empty on the page. | the cell at address 3000 holds 2000; the cell at address 2000 holds 10 |
| P2-N1 | 19 / 18 | "Pointer is used to points the address of the value stored anywhere in the computer memory" | terminology | **IMPRECISE** | NEW | A pointer is a *variable that stores an address* (of an object of a given type). It does not "point the address". | "A pointer is a variable whose value is the address of another object." |
| P2-N2 | 19 / 18 | Diagram under "Pointer to pointer" heading shows only `b = &a` | diagram | **IMPRECISE** | NEW | The figure under the "pointer to pointer" heading shows a single-level pointer. The `int **` level is not drawn. | Add `c = &b` (a cell at, e.g., address 4000 holding 3000). |
| P2-03 | 20, 21 / 19, 20 | `printf ("address of a = %u \n", &a);` | UB | **ERROR** | CONFIRMED | Wrong argument type for `%u` is UB (C11 7.21.6.1p9). clang with `-Wformat` warns (verified). | `%p` with `(void *)` |
| P2-04 | 20 / 19 | `printf ("address of a = %d \n", b);` | UB | **ERROR** | CONFIRMED | Same UB. The arithmetic checks out: 3010494292 − 2³² = −1284473004, which matches the printed value. | `%p`, `(void *)b` |
| P2-05 | 20 / 19 | `printf("address of b = %u\n", &b)` | compile error | **ERROR** | CONFIRMED | The missing `;` is clearly visible on the page (every other line has one). | add `;` and use `%p` |
| P2-06 | 20 / 19 | output: 5 lines for 7 printf calls | inconsistency | **ERROR** | CWC | **Confirmed:** only two "value of a = 5" lines are shown for three printf calls. Moreover, the program as written does not compile (P2-05). **Rejected sub-claims:** (1) "the last printf has no `\n` but appears on its own line in the output" — the *previous* printf ends with `\n`, so the last one does start on a new line. (2) "a 64-bit run would not show 32-bit addresses / &a and &b 4 bytes apart" — `%u` prints the low 32 bits of the pointer (reproduced: exactly the low 32 bits of the `%p` value are printed). 3010494296 is divisible by 8, so a 4-byte `int` at …292 and an 8-aligned pointer at …296 is an ordinary x86-64 stack layout. The listing is plausibly real (truncated) output. | Show three "value of a = 5" lines; print with `%p`. |
| P2-07 | 21 / 20 | `printf ("address of b = %u \n", c);` / outputs …116, …120, …128 | UB | **IMPRECISE** | CWC | The UB from `%u` is real (duplicate of P2-03). The "inconsistency / values look made up" part is rejected: the 8-byte gap between b and c and the truncation to the low 32 bits via `%u` are exactly what a 64-bit machine produces. Printing `c` as "address of b" is correct, since c == &b. | `%p` |

<a id="04"></a>

## 04. Structures (PDF 22–23)

| ID | PDF/nb. page | Quote | Category | Severity | Verdict | Why it is wrong | Correct version |
|---|---|---|---|---|---|---|---|
| P2-08 | 22, 24, 27 / 21, 23, 26 | `void main ()` | non-standard C | **ERROR** | CONFIRMED | Not a strictly conforming form of `main` for a hosted environment (C11 5.1.2.2.1 only allows implementation-defined alternatives). In practice clang rejects it: "error: 'main' must return 'int'" (verified). GCC warns. | `int main(void) { … return 0; }` |
| P2-09 | 22–23 / 21–22 | `#include <conio.h>` … `getch ();` | non-standard C | **ERROR** | CONFIRMED | DOS/Turbo C only. Not part of ISO C. | `getchar()` |
| P2-10 | 22 / 21 | `int mobile;` | overflow | **ERROR** | CONFIRMED | A 10-digit number exceeds INT_MAX. An out-of-range `%d` conversion is UB (C11 7.21.6.2p10). | `char mobile[16]` with `%15s` |
| P2-11 | 23 / 22 | `printf (%d %f %d", &e3.id, &e3.salary, &e3.mobile);` | compile error + code bug | **ERROR** | CONFIRMED | Checked at 220 dpi: the opening `"` is missing. It should be `scanf`. As written, e3 is never read, so printing e3 reads indeterminate values. | `scanf("%d %f %d", &e3.id, &e3.salary, &e3.mobile);` |
| P2-12 | 23 / 22 | `scanf ("%d %f %d ", …e2…)` | code bug | **IMPRECISE** | CONFIRMED | The trailing space in the format is visible at 220 dpi. A trailing whitespace directive blocks until a non-whitespace character arrives. | remove the trailing space |
| P2-13 | 23 / 22 | scanf return unchecked | code bug | **IMPRECISE** | CONFIRMED | Style/robustness. | check `== 3` |

<a id="05"></a>

## 05. Arrays (PDF 24–31)

| ID | PDF/nb. page | Quote | Category | Severity | Verdict | Why it is wrong | Correct version |
|---|---|---|---|---|---|---|---|
| P2-14 | 24 / 23 | `int marks_1 = 56; marks_2 = 78, marks_3 = 89;` | compile error | **ERROR** | CONFIRMED | The `;` after 56 is clearly visible. marks_2 and marks_3 are undeclared. | commas |
| P2-15 | 24 / 23 | `Float avg = …` | compile error | **ERROR** | CONFIRMED | The capital F is visible. | `float` |
| P2-16 | 24 / 23 | `(marks_1 + marks_2 + marks_3)/3` | code bug | **ERROR** | CONFIRMED | Integer division: 223/3 = 74. | `/ 3.0f` |
| P2-17 | 24 / 23 | `print (avg);` | compile error | **ERROR** | CONFIRMED | C has no `print` function. | `printf("%f\n", avg);` |
| P2-18 | 24 / 23 | `void main` (no parentheses) | compile error | **ERROR** | CONFIRMED | There is no `()` on the page. clang: "variable has incomplete type 'void'" (verified). | `int main(void)` |
| P2-19 | 24–25 / 23–24 | `float avg;` … `avg = avg + marks [i];` | UB | **ERROR** | CONFIRMED | Uninitialized variable. The address of avg is never taken, so reading it is UB per C11 6.3.2.1p2. | `float avg = 0;` |
| P2-N3 | 24 / 23 | `int arr[10]; char arr[10]; float arr[5]` | compile error if taken as code | **IMPRECISE** | NEW | Read as a single block, `arr` is redeclared with incompatible types (constraint violation, C11 6.7p3), and the last declaration lacks `;`. Intended as alternatives. | Use distinct names, or present them as separate alternatives on separate lines, each with `;`. |
| P2-20 | 25 / 24 | `printf (avg);` and no division by 3 | compile error + code bug | **ERROR** | CONFIRMED | clang: "passing 'float' to parameter of incompatible type 'const char *'" (verified). The "average" is never divided by 3. | `avg /= 3; printf("%f\n", avg);` |
| P2-21 | 25 / 24 | "In Array space complexity for worst case is O(n)" | complexity | **IMPRECISE** | CONFIRMED | Does not say whether storage or auxiliary space is meant. | "Storage: O(n); auxiliary space per operation: O(1)." |
| P2-22 | 25 / 24 | Insertion/Deletion O(n)/O(n) | complexity | **IMPRECISE** | CONFIRMED | The end-of-array case is O(1). Search in a sorted array is O(log n). | Add these cases (insert/delete at the end O(1); search in a sorted array O(log n)). |
| P2-23 | 25–26 / 24–25 | "Indexing of array can be defined in three ways" | terminology | **IMPRECISE** | CONFIRMED | C arrays are always 0-indexed. | State that C indexes only from 0; other bases are conceptual or come from other languages (Pascal, Fortran). |
| P2-24 | 26 / 25 | "Byte address of element A[i] = base address + size * (first - index)" | formula | **ERROR** | CONFIRMED | The page clearly says "(first - index)". The sign is reversed. The example on p. 27 uses (index − first): 999+2·9 = 1017. | `BA + size·(i − LB)` |
| P2-25 | 27 / 26 | "The name of the array represents the starting address" | factual | **IMPRECISE** | CONFIRMED | The array name decays to a pointer except as the operand of sizeof, unary &, or a string-literal initializer (C11 6.3.2.1p3). | "In most expressions an array name decays to a pointer to its first element." |
| P2-26 | 27 / 26 | `int summation (int arr[])` … `i<5` | code bug | **IMPRECISE** | CONFIRMED | The length is hard-coded. The parameter is actually a pointer (adjusted to pointer). | pass the length as a parameter |
| P2-29 | 29 / 28 | `int arr[2][2] = {0,1,2,3};` | style | **IMPRECISE** | CONFIRMED | Valid C. clang with `-Wall` emits `-Wmissing-braces` (verified). | `{{0,1},{2,3}}` |
| P2-30 | 29 / 28 | "The size of a two dimensional array is equal to the multiplication of number of rows and number of coloumns" | factual | **IMPRECISE** | CONFIRMED | That is the element count. The size in bytes must also be multiplied by the element size. | `sizeof arr = m × n × sizeof(arr[0][0])` |
| P2-31 | 30 / 29 | a11…a33 diagrams vs 0-based formula | inconsistency | **IMPRECISE** | CONFIRMED | Mixes indexing conventions (1-based diagrams a11…a33 and a 0-based formula). | State "i, j from 0" or use `BA + ((i−LR)·n + (j−LC))·size`. |
| P2-32 | 30 / 29 | "Address (a[i][j] = (j * m)+i)*size+B.A." | typo | **IMPRECISE (downgraded)** | CWC | Checked at 300 dpi. The parentheses are *balanced* (two opening: "Address (" and "(j"; two closing: "m)" and "i)"), so the original "unbalanced" claim is false. The error is a *misplaced* parenthesis: the group "(a[i][j] = (j*m)+i)" swallows the `=` sign. The intended formula (0-based indices, m rows) is clear and correct. Severity downgraded. | `Address(a[i][j]) = BA + (j·m + i)·size` |
| P2-33 | 31 / 30 | "3 * 4 = 12 bytes" | factual | **IMPRECISE** | CONFIRMED | Assumes a 4-byte int. | "3·sizeof(int)" |
| P2-34 | 31 / 30 | "we are providing fixed-size at compile time, due to which wastage of memory occurs" | factual | **IMPRECISE** | CONFIRMED | malloc/realloc and VLAs exist (VLAs are optional in C11). Linked lists also have overhead — a pointer in every node. | Restrict the claim to static arrays; mention dynamic arrays and the per-node pointer overhead of linked lists. |

<a id="06"></a>

## 06. Linked lists (PDF 31–37)

| ID | PDF/nb. page | Quote | Category | Severity | Verdict | Why it is wrong | Correct version |
|---|---|---|---|---|---|---|---|
| P2-35 | 32, 33, 34 (×2) / 31, 32, 33 | `struct node { … }` with no `;` | compile error | **ERROR** | CONFIRMED | All four occurrences confirmed on the images. The versions on pp. 35/36 have `};`. clang: "expected ';' after struct" (verified). | `};` |
| P2-36 | 32 / 31 | "In linked list, one is variable and second one is pointer variable" | terminology | **IMPRECISE** | CONFIRMED | Vague wording. | "Each node has a data field and a pointer (link) field to the next node." |
| P2-37 | 33 / 32 | "The only difference is 'last node does not point to any node in a singly linked list'" | factual | **IMPRECISE** | CWC | As a contrast it is not false: it correctly says that in a *singly* linked list the last node points nowhere, which implies that in a circular one it does. Incomplete, because it never says the last node points back to the head. The original "describes SLL, not the difference" is an overstatement. | "…whereas in a circular list the last node's next points to the first node." |
| P2-38 | 34 / 33 | "does not contain NULL value in previous field of the node" | factual | **IMPRECISE** | CONFIRMED | In a non-empty DCLL neither prev nor next is NULL. | "There is no NULL in either the prev or the next field: head->prev = tail and tail->next = head." |
| P2-39 | 35 / 34 | SLL Insertion O(1), Deletion O(1) (avg and worst) | complexity | **IMPRECISE** | CONFIRMED | O(1) only at the head, or when the predecessor is already known. | "Insert/delete at head: O(1). At the end/at a position: O(n) (append is O(1) with a tail pointer). Search/access: O(n). Space: O(n)." |
| P2-40 | 35 / 34 | `ptr = (struct node *) malloc(sizeof(struct node *)` | code bug / UB + compile error | **ERROR** | CONFIRMED | Confirmed on the image: `sizeof(struct node *)` (the size of a pointer), the `malloc(` parenthesis is not closed, and there is no `;`. On LP64 sizeof(struct node) = 16, while a pointer is 8. | `ptr = malloc(sizeof *ptr); if (!ptr) …` |
| P2-41 | 35 / 34 | `struct node *head, *ptr;` | code bug | **IMPRECISE** | CWC | The fragment must be at block scope because a statement follows, so `head` really is indeterminate. But `head` is not used in the fragment, so this is a latent hazard rather than a bug in the existing code. | `struct node *head = NULL, *ptr;` |
| P2-42 | 36 / 35 | "The list can either be empty or full" | factual | **ERROR** | CONFIRMED | A linked list has no "full" state. The cases are: empty, one node, more than one node. | "Cases: empty list, one node, more than one node." |
| P2-43 | 36 / 35 | "visit each node of the list at least once" | factual | **IMPRECISE** | CONFIRMED | Traversal visits every node exactly once. | "exactly once" |
| P2-44 | 36 / 35 | "If the element is found on any of the location of that element is returned otherwise null is returned" | terminology | **IMPRECISE** | CONFIRMED | Incoherent sentence. | "If found, return a pointer to the node (or its index); otherwise return NULL (or −1)." |
| P3-01 | 37 / 36 | "visiting each node of the list atleast once" | terminology | **IMPRECISE** | CONFIRMED | The quote matches the page. Traversal visits every node exactly once. | "…exactly once" |

<a id="07"></a>

## 07. Skip list (PDF 37–41)

| ID | PDF/nb. page | Quote | Category | Severity | Verdict | Why it is wrong | Correct version |
|---|---|---|---|---|---|---|---|
| P3-02 | 37 / 36 | "used to store a linked list of elements or data with a linked list" | factual | **IMPRECISE** | CONFIRMED | The definition is circular and says nothing about multi-level forward pointers (Pugh 1990). | "A sorted linked list plus a hierarchy of increasingly sparse 'express lists'" |
| P3-03 | 37 / 36 | "skip list is built in two layers: The lowest layer and the top layer" | factual | **IMPRECISE (downgraded)** | CWC | The quote is on the page. Literally, "two layers" is false: Pugh's skip list has up to MaxLevel levels, about log_{1/p} n in expectation. The next sentence says "top layers" (plural), so the author apparently means two *kinds* of layer: the base list and the express layers. Misleading, but not an outright contradiction. The 4-level example on pp. 40–41 shows the real structure. Severity downgraded from ERROR. | "Level 0 is a sorted list of all elements; levels 1…L are increasingly sparse 'express lanes'" |
| P3-04 | 38 / 37 | Space: Average "—", Worst "O(n log n)" | complexity | **IMPRECISE** | CONFIRMED | Checked on the image: the average-case cell is a dash. Expected space is O(n) (expected tower height 1/(1−p)). The O(n log n) worst case holds if the height is capped at about log n. | average O(n), worst O(n log n) |
| P3-05 | 38–39 / 37–38 | "Insertion (L, key)" … "make node (lvl, key, value)" | inconsistency | **ERROR** | CONFIRMED | Both quotes are on the pages. `value` is never passed in. | `Insert(L, key, value)` |
| P3-06 | 38, 39 / 37, 38 | "local update [0 ... max-level +1]" | code (bounds) | **IMPRECISE** | CWC | The quote is on the page. Two more cells are allocated than needed (0…MaxLevel−1, or 1…MaxLevel in Pugh). The extra cells are harmless, so this is not out-of-bounds, just a mix of 1-based and 0-based indexing. | `update[0 .. MaxLevel-1]` |
| P3-07 | 38, 39, 40 / 37–39 | "while a → forward [i] → key forward [i]" | code bug | **ERROR** | CONFIRMED | The same garbled line appears in all three algorithms (insert, delete, search). The `< key` comparison and the advance `a := a→forward[i]` are missing. | `while a→forward[i]→key < key do a := a→forward[i]` |
| P3-08 | 39 / 38 | No `if a→key = key` check after "a = a → forward[0]" | code bug | **IMPRECISE (downgraded)** | CWC | Pugh's dictionary insert updates the value of an existing key; a multiset skip list may allow duplicates. Since the algorithm stores (key, value) pairs it is meant as a dictionary, so the check is missing, but this is an omission rather than clearly wrong code. Severity downgraded. | `if a→key = key then a→value := value; return` |
| P3-09 | 39 / 38 | "if lvφ > L → level then" | meaningful typo | **ERROR** | CONFIRMED | The zoomed image shows "lvφ". `lvl` is meant. | `if lvl > L→level` |
| P3-10 | 39 / 38 | "for i = L→level+1 to lvl do / update[i] = L→header / L→level = lvl" | code bug | **IMPRECISE** | CWC | `L→level = lvl` is indented like the loop body but belongs after the loop. The correct upper bound depends on the convention. The example (pp. 40–41: "level 4" means 4 pointer cells, 0–3) treats lvl as a **count**, giving `to lvl−1`. The search loop ("L→level down to 0") treats level as a **top index**, giving `to lvl`. The notes mix both (see P3-15). | Pick one convention. For count semantics: `for i = L→level+1 to lvl−1 …; L→level = lvl−1` (index) |
| P3-11 | 39 / 38 | "for i = 0 to level do / a→forward[i] = … / update[i]→forward[i] = a" | code bug | **ERROR** | CONFIRMED | `level` is undefined here. The loop should run over the new node's levels. The second assignment is written outside the loop. | `for i = 0 to lvl−1 (count) do { a→forward[i] = update[i]→forward[i]; update[i]→forward[i] = a }` |
| P3-12 | 39 / 38 | "a = L ↛ header" | typo | **IMPRECISE** | CONFIRMED | The arrow is crossed by a stray stroke. The meaning is obvious. | `a = L→header` |
| P3-13 | 39 / 38 | "if update[i]→forward[i] ? a then break" | code bug | **IMPRECISE** | CONFIRMED | At 300 dpi the relational symbol is an illegible glyph resembling "?". It should be ≠. | `≠ a` |
| P3-14 | 39 / 38 | "update [i] → forward [i] → forward [i]" | code bug | **ERROR** | CONFIRMED | There is no assignment, and the node being deleted is never referenced. | `update[i]→forward[i] = a→forward[i]` |
| P3-15 | 39 / 38 | "while L→level > 0 and L→header→forward[L→level] = NIL" | inconsistency | **IMPRECISE** | CWC | The loop is correct if `L→level` is a top index. However, the original entry's reasoning is wrong: "for i = 0 to L→level" is inclusive, so it also treats level as an index. The real inconsistency is with the count semantics of the example ("level 4" = 4 cells) and with `L→level = lvl` when lvl is a count: then `forward[L→level]` indexes past the header's top cell. | One convention throughout |
| P3-NEW-2 | 39 vs 40–41 / 38 vs 39–40 | random-level / "level k" semantics | inconsistency | **IMPRECISE** | NEW | In the example, "level k" means a tower with k pointer cells (0…k−1). In the algorithm, "for i = L→level down to 0" treats level as a top index, and then `L→level = lvl` makes the list level one higher than the header has. This is the root cause of P3-10, P3-11, and P3-15. | Fix one convention: lvl = number of levels, L→level = highest index |
| P3-16 | 40 / 39 | "loop invariant : a → key level down to 0 do." | code bug | **ERROR** | CONFIRMED | The `for i = L→level` header is merged with the invariant, so `i` is never defined. | `for i = L→level downto 0 do` (invariant `a→key < skey`) |
| P3-17 | 40 / 39 | "a = a → forward [a]" | meaningful typo | **ERROR** | CONFIRMED | The image clearly shows `[a]`. | `a = a→forward[0]` |
| P3-NEW-1 | 40 / 39 | Skip-list example steps 1–2: header pointers drawn at the level-1 (step 1) and level-2 (step 2) rows | diagram | **IMPRECISE** | NEW | Zoomed at 200 dpi. Nodes 6 and 29 are drawn as [level-0 cell + key cell], but the arrows from the header (and from 6 to 29) are on the level-1 or level-2 row instead of level 0. Steps 3–6 draw them correctly at level 0. | Draw header→6→29 on row 0 |

<a id="08"></a>

## 08. Stack (PDF 42–44)

| ID | PDF/nb. page | Quote | Category | Severity | Verdict | Why it is wrong | Correct version |
|---|---|---|---|---|---|---|---|
| P3-18 | 42 / 41 | "Abstract Data Type with a pre-defined capacity" | factual | **IMPRECISE** | CONFIRMED | Capacity is a property of the array implementation. The ADT has no capacity. | "A stack is an ADT; the array implementation has a fixed capacity (overflow is possible), the linked one does not." |
| P3-19 | 42 / 41 | "peek () :- It returns the element at a given position." | terminology | **ERROR** | CONFIRMED | Standard peek()/top() return the top element. The positional version is usually called "peep(i)". The notes' own peek for queues (p. 46) uses the standard meaning. | "returns the top element without removing it" |
| P3-20 | 43 / 42 | Push fig: "Stack is full", top=2, with empty space above 30 | diagram | **IMPRECISE** | CONFIRMED | Each block is drawn about 4 cells tall. Block 4 still has a free cell above 30. | a 3-cell block, or drop "full" |
| P3-21 | 43 / 42 | "once the top operation is performed" | terminology | **IMPRECISE** | CONFIRMED | The pop operation is meant. | "after a pop operation, top = top − 1" |
| P3-22 | 43 / 42 | Pop fig labels: top=1, top=-1, top=-1, top=-1 | diagram | **ERROR** | CWC | The labels are as quoted. The push figure labels each block with the top of the stack **drawn in it** (−1, 0, 1, 2). By the same convention the pop blocks [10,20,30], [10,20], [10], and [] should be 2, 1, 0, −1, so **three of the four** labels are wrong, not just the second. The original entry's reading (label = top after pop) is less charitable to the figure, but it also finds label 2 wrong. | top = 2, 1, 0, −1 |
| P3-23 | 44 / 43 | "the compiler creates a system stack" | factual | **IMPRECISE** | CONFIRMED | The call stack exists at run time. The compiler only generates the code that manages it. | "at run time each call pushes an activation record (frame) onto the call stack" |
| P3-24 | 44 / 43 | "if top = n then stack full / top = top +1 / stack(top) := item" | code bug | **ERROR** | CONFIRMED | Nothing stops execution after the full check, so it falls through and writes. The 1-based check also contradicts "top = −1 means empty" on p. 43. | `if top = n−1 then overflow; return` |
| P3-25 | 44 / 43 | "if top = 0 then empty; item := stack(top); top = top − 1" | code bug | **ERROR** | CONFIRMED | Same pattern: no exit on underflow, and the 1-based check contradicts top = −1 as the empty marker. | `if top = −1 then underflow; return` |

<a id="09"></a>

## 09. Queue (PDF 45–48)

| ID | PDF/nb. page | Quote | Category | Severity | Verdict | Why it is wrong | Correct version |
|---|---|---|---|---|---|---|---|
| P3-26 | 45 / 44 | "The deque operation can also be designed to void." | terminology | **IMPRECISE** | CONFIRMED | "Deque" is a different structure (double-ended queue), and dequeue usually returns the element. | "Dequeue usually returns the removed element; it can also be designed to return void (with a separate peek())." |
| P3-27 | 46 / 45 | [10\|20\|30\| ], "Rear ↑" under 20 | diagram | **ERROR** | CONFIRMED | On the image the Rear arrow is clearly under the 20 cell. | Rear under 30 |
| P3-28 | 46 / 45 | "drawback … insertion is done only from rear end" | factual | **IMPRECISE** | CONFIRMED | Inserting at the rear is the definition of a queue, not a drawback. The real drawback is false overflow. | "Drawback: when REAR = MAX−1 the queue reports overflow even if cells before FRONT are free (wasted space)." |
| P3-29 | 47 / 46 | "by simply incrementing value of rear" | factual | **IMPRECISE** | CONFIRMED | The increment must wrap around: `(rear+1) mod MAX`. | "… via rear = (rear + 1) mod MAX" |
| P3-31 | 48 / 47 | "value of front will increase from -1 to 0" | factual | **ERROR** | CONFIRMED | Before the deletion FRONT = 0 (at H). After it, FRONT = 1, which matches the page's own figure. | "from 0 to 1" |
| P3-32 | 48 / 47 | "Go to step  [END OF IF]" | meaningful typo | **ERROR** | CONFIRMED | The target step number is missing. | "Go to Step 4" |

<a id="10"></a>

## 10. Trees (PDF 49–58)

| ID | PDF/nb. page | Quote | Category | Severity | Verdict | Why it is wrong | Correct version |
|---|---|---|---|---|---|---|---|
| P3-33 | 50 / 49 | "If the node is a descendant of any node, then node is called as child node." | terminology | **ERROR** | CONFIRMED | A child is an *immediate* descendant. | "A node directly connected below another node (its parent) is a child of that node." |
| P3-34 | 50 / 49 | "a leaf a bottom-most node of tree" | terminology | **IMPRECISE** | CONFIRMED | A leaf is not necessarily on the bottom level (e.g. node 3 on p. 52). | "a leaf is a node with no children (degree 0)" |
| P3-35 | 50 / 49 | "1, 2, 5 are ancestors of node 10" | diagram / factual | **ERROR** | CONFIRMED | The figure on p. 49 contains only nodes 1–8, and 5 is a leaf. | "1, 2 are ancestors of node 5" |
| P3-36 | 50 / 49 | "The immediate successor … is known as descendant" | terminology | **ERROR** | CONFIRMED | That is the definition of a child, not a descendant. | "Any node in the subtree of x (other than x itself) is a descendant of x." |
| P3-37 | 50 / 49 | "each node, except root node, will have atleast one incoming link" | factual | **IMPRECISE (downgraded)** | CWC | For trees the statement is **true** ("exactly one" implies "at least one"). It is merely too weak to justify the n−1 edge count. Not clearly false. Severity downgraded. | "exactly one incoming edge" |
| P3-38 | 51 / 50 | `Struct node { … }` with no `;` | compile error | **ERROR** | CWC | The missing `;` is confirmed on the image and by clang (`expected ';' after struct`). The capital "S" in "Struct" is **uncertain**: this author's lowercase s often looks like a capital, so that part of the claim is unreliable. The missing `;` alone is enough for a compile error. | `struct node { …; };` |
| P3-39 | 51 / 50 | "can only be defined for binary trees … and generic trees." | factual | **IMPRECISE** | CONFIRMED | The sentence is unfinished. The left-child/right-sibling representation lets general trees be stored in the same structure. | "With two child pointers this directly models a binary tree; a general tree needs a child list or the left-child/right-sibling representation." |
| P3-40 | 51 / 50 | Fig: [ \| B \| x ] and [ \| C \| ] | diagram | **IMPRECISE** | CONFIRMED | Visible on the image. The NULL markers in the two leaves are inconsistent. | [x\|B\|x], [x\|C\|x] |
| P3-41 | 52 / 51 | "The children of parent node are known as subtree." | terminology | **IMPRECISE** | CONFIRMED | A child is a node. A subtree is a child **together with all its descendants**. | "each child is the root of a subtree" |
| P3-42 | 52 / 51 | "subtrees are unordered as nodes in subtree cannot be ordered" | factual | **IMPRECISE** | CONFIRMED | Ordered trees are a standard notion (CLRS B.5.2). | "In a general tree the order of subtrees may or may not matter (ordered vs unordered tree)." |
| P3-43 | 52 / 51 | "Every non empty tree has a downward edge" | factual | **IMPRECISE (downgraded)** | CWC | Fails only for the single-node tree. A genuine counterexample, but an edge case. Severity downgraded. | "every tree with ≥ 2 nodes…" |
| P3-44 | 53 / 52 | "h = log₂(n+1) − 1" | complexity | **IMPRECISE** | CONFIRMED | n = 4 gives 1.32, but the actual minimum height is 2 = ⌈log₂5⌉ − 1 = ⌊log₂4⌋. | ⌈log₂(n+1)⌉ − 1 |
| P3-45 | 54 / 53 | "except left nodes" | meaningful typo | **ERROR** | CONFIRMED | The image says "left". "leaf" is meant. | "leaf nodes" |
| P3-46 | 54 / 53 | "minimum number of nodes : 2 * h − 1" | inconsistency | **ERROR** | CONFIRMED | If height is counted in edges (as in the 2^(h+1)−1 formula from the same list), the minimum is 2h+1. E.g. h = 2 (A–B–D/E–C) gives 5 nodes, not 3. | 2h + 1 |
| P3-47 | 54 / 53 | "maximum height h = (n+1)/2" | inconsistency | **ERROR** | CONFIRMED | The page's own example, n = 5, has height 2 = (n−1)/2, not 3. | (n − 1)/2 |
| P3-48 | 54 / 53 | full BT "minimum height log₂(n+1) − 1" | complexity | **IMPRECISE** | CONFIRMED | n = 5 gives 1.58, but the minimum height is 2. | ⌈log₂(n+1)⌉ − 1 |
| P3-49 | 54 / 53 | "all nodes are completely filled except the last level" | terminology | **IMPRECISE** | CONFIRMED | It is the levels that are completely filled. The last level is filled from the left. | "All levels except possibly the last are completely filled, and the nodes of the last level are as far left as possible." |
| P4-01 | 55 / 54 | complete BT "minimum height → log₂(n+1) − 1" | complexity | **IMPRECISE** | CONFIRMED | A complete tree with n nodes has height exactly ⌊log₂n⌋. Example: n = 6 → 2. | ⌊log₂ n⌋ |
| P4-02 | 55 / 54 | "vice versa is not true, all complete binary trees and full binary trees are the perfect" | factual / inconsistency | **ERROR** | CONFIRMED | The sentence denies the converse and then asserts it. The trees on p. 54 are counterexamples. | "not all complete/full trees are perfect" |
| P4-03 | 55 / 54 | "both left and right trees by almost 1" | terminology | **IMPRECISE** | CONFIRMED | Garbled wording. It should say: the heights differ by at most 1, and this holds at every node. | A tree is height-balanced if \|h(L) − h(R)\| ≤ 1 at every node. |
| P4-04 | 55 / 54 | "diff betⁿ left subtree & right S.T. is zero" | factual | **IMPRECISE** | CONFIRMED | Recomputed: at the root the subtree heights are 1 and 1 (difference 0). At node 5 they are −1 and 0 (difference 1). The tree is balanced because \|BF\| ≤ 1 everywhere, not because the root difference is 0. | The tree is balanced because \|BF\| ≤ 1 at every node. |
| P4-05 | 56 / 55 | `struct node { … }` with no `;` | compile error | **ERROR** | CONFIRMED | Visible on the image and verified by compiling: `error: expected ';' after struct`. | `};` |
| P4-06 | 56 / 55 | "three types of traversals" | factual | **IMPRECISE** | CONFIRMED | Level-order traversal is missing. | Also mention level-order traversal (BFS). |

<a id="11"></a>

## 11. Tree types: BST/AVL/B/B+ (PDF 58–63)

| ID | PDF/nb. page | Quote | Category | Severity | Verdict | Why it is wrong | Correct version |
|---|---|---|---|---|---|---|---|
| P4-07 | 56 / 55 | BST defn has only the "similarly … right subtree ≥ root" clause | factual (incomplete) | **ERROR** | CONFIRMED | The left-subtree condition is missing on the page, so "similarly" refers to nothing. | Add "left subtree < root"; the definition is recursive |
| P4-08 | 57 / 56 | "insert it as root of left sub-tree … root of right of right subtree" | factual | **IMPRECISE** | CONFIRMED | A new key is inserted as a leaf after a recursive descent. The listed steps 1–9 match the figures. | If key < node go left, otherwise go right; repeat until a NULL link and insert the key there as a leaf. |
| P4-NEW-1 | 59 / 58 | AVL example, node 50 annotated "1−2=1" | diagram / arithmetic | **IMPRECISE** | NEW | Zoomed at 300 dpi: the annotation reads "1−2=1" (or "=)"), with no minus sign. The balance factor is −1. The text ("between −1 and +1") is correct. The transcript silently corrected this to "−1". | "1−2 = −1" |
| P4-09 | 60 / 59 | "By limiting this height to log n" | complexity | **IMPRECISE** | CONFIRMED | AVL tree height < 1.4405 log₂(n+2) − 0.328 (Knuth 6.2.3). The bound is O(log n), not exactly log n. | Height is O(log n), at most ≈1.44 log₂ n, so every operation is O(log n). |
| P4-10 | 61 / 60 | "Inserted node is in the left subtree of left subtree of A" | terminology | **IMPRECISE** | CONFIRMED | A is never defined, and the case names describe the kind of imbalance, not the rotation that fixes it. | Define A (the nearest ancestor of the new node with \|BF\| = 2) and name the rotation for each case. |
| P4-11 | 61 / 60 | "at least m/2 children" | factual | **IMPRECISE** | CONFIRMED | The bound is ⌈m/2⌉. | ⌈m/2⌉ |
| P4-12 | 61 / 60 | "The root nodes must have at least 2 nodes." | factual | **ERROR** | CONFIRMED | The constraint is on children, not nodes, and a root that is a leaf is an exception. | "the root, if it is not a leaf, has ≥ 2 children" |
| P4-13 | 62 / 61 | "compare item 49 with root node 78 … 40<49<56 … 49>45" | diagram / inconsistency | **ERROR** | CONFIRMED | The "following B Tree" is never drawn. The only B-tree in the notes (p. 61) has root 60 and contains neither 49 nor 78. | Draw the referenced tree or redo the example on the tree from p. 61. |
| P4-14 | 62 / 61 | "similar to searching in Binary tree" / "traverse right subtree of 40" | terminology | **IMPRECISE** | CWC | "Binary tree" should be "binary **search** tree" — this part is valid. The second half of the original entry is wrong: in a B-tree, the child pointer immediately to the right of key 40 *is* the subtree between 40 and 56, so "right subtree of 40" is acceptable wording. | "similar to searching in a BST" |
| P4-16 | 62 / 61 | "split it too by steps" | factual | **IMPRECISE** | CONFIRMED | The root split, which increases the height, is missing. | Add: "If the root splits, the median becomes the new root (height + 1)." |
| P4-17 | 63 / 62 | "Height … less as compare to B tree" | factual | **IMPRECISE** | CONFIRMED | Height is usually smaller because of the higher fan-out, but this is not guaranteed. | "Since internal nodes store only keys, the fan-out is higher, so for the same page size the height is usually smaller." |

<a id="12"></a>

## 12. Graphs (PDF 63–69)

| ID | PDF/nb. page | Quote | Category | Severity | Verdict | Why it is wrong | Correct version |
|---|---|---|---|---|---|---|---|
| P4-19 | 63 / 62 | "an ordered set G(V, E)" | terminology | **IMPRECISE (downgraded)** | CWC | The correct term is "ordered pair". "Ordered set G(V,E)" is loose wording common in sources, and the meaning is clear. Severity downgraded. | G = (V, E) is an ordered pair |
| P4-20 | 65 / 64 | "simple path :- … with an exception V₀ = V_N … closed simple path" | factual | **ERROR** | CONFIRMED | Under the "simple path" heading the page defines a closed simple path and says "nodes of graph" instead of "nodes of the path". | Simple path: all vertices of the path are distinct. Closed simple path: the same, but V₀ = V_N. |
| P4-21 | 65 / 64 | "Cycle :- … no repeated edges or vertices except first and last" | factual | **IMPRECISE** | CONFIRMED | Does not require V₀ = V_N, nor length ≥ 3 for an undirected graph. | A cycle is a closed path of length ≥ 1 (≥ 3 for an undirected graph) with no repeated vertices except V₀ = V_N. |
| P4-22 | 65 / 64 | "There are no isolated nodes in connected graph." | factual | **IMPRECISE** | CWC | The K₁ exception is valid. The original entry's extra remark ("having no isolated vertices does not imply connectivity") answers a claim the notes do not make. | "…with ≥ 2 vertices" |
| P4-23 | 65 / 64 | "complete graph contain n(n−1)/2 edges" | factual | **IMPRECISE** | CONFIRMED | Correct only for simple undirected graphs. A complete digraph has n(n−1) edges. | n(n−1)/2 (undirected), n(n−1) (directed). |
| P4-24 | 65 / 64 | "each node is assigned with some data such as length or width" | factual | **ERROR** | CONFIRMED | Weights are assigned to edges. The next line on the same page, w(e), says exactly that. | Each **edge** is assigned a weight. |
| P4-25 | 65 / 64 | "w(e) which must be positive (+) value" | factual | **ERROR** | CONFIRMED | In general, zero and negative weights are allowed (Bellman–Ford, CLRS ch. 24). | w(e) is a real number; some algorithms (e.g. Dijkstra) require w(e) ≥ 0. |
| P4-26 | 65 / 64 | "Diagraph" | spelling (term) | **IMPRECISE (downgraded)** | CWC | A misspelling of "digraph". The adjacent definition makes the meaning unambiguous. Severity downgraded. | Digraph |
| P4-27 | 66 / 65 | "degree … number of edges connected with that node" | factual | **IMPRECISE** | CONFIRMED | Self-loops count twice, and in directed graphs degree splits into in-degree and out-degree. | deg(v) = number of incident edges, self-loops counted twice. Digraph: in-degree and out-degree. |
| P4-28 | 67 / 66 | weighted matrix uses 0 for "no edge" | factual | **IMPRECISE** | CONFIRMED | If 0 means "no edge", an edge of weight 0 cannot be distinguished from a missing one. Matrix values were checked against the figure: they match. | ∞ / NIL for a missing edge |

<a id="13"></a>

## 13. Graph traversals / spanning trees (PDF 69–72)

| ID | PDF/nb. page | Quote | Category | Severity | Verdict | Why it is wrong | Correct version |
|---|---|---|---|---|---|---|---|
| P4-29 | 68 / 67 | "examining all nodes and vertices of graph" | terminology | **IMPRECISE (downgraded)** | CWC | Redundant rather than wrong (probably "vertices and edges" was meant). Severity downgraded. | "examining all vertices and edges of the graph" |
| P4-30 | 68 / 67 | "starts traversing graph from root node … until it finds goal" | terminology | **IMPRECISE** | CONFIRMED | Graphs have a source vertex, not a root, and BFS traversal visits everything reachable. | "starts at a source vertex s and visits all vertices reachable from s in order of distance" |
| P4-31 | 69 / 68 | lists "B : C, F", "E : B, F" vs. drawing | diagram / inconsistency | **ERROR** | CONFIRMED | Zoomed at 300 dpi. Drawn edges: A→B, B→C, C→G, C→E, G→E, E→F, D→F, F→A, A→D. There is neither B→F nor E→B. BFS rerun on both versions: dist(E) = 3 via A→B→C→E. | Make them consistent: either draw B→F and E→B, or use the lists B: C and E: F. The answer A→B→C→E is correct for both versions. |
| P4-32 | 69 / 68 | Solution "A → B → C → E" from status-only BFS | factual | **IMPRECISE** | CONFIRMED | The algorithm as written does not record predecessors, so it cannot output a path. | store PRED[v] |
| P4-33 | 70 / 69 | DFS step 5 marks STATUS=2 when pushed | factual | **IMPRECISE** | CONFIRMED | Traced by hand on A–B, A–C, A–D, B–D: pop A and push B, C, D (all marked); pop D (B already marked); pop C; pop B. Order A D C B. This is not a valid DFS order: after D the search should go to D's unvisited neighbor B (A D B C). | mark on pop / recursive DFS |
| P4-34 | 70 / 69 | "until we find goal node / node which has no children" | terminology | **IMPRECISE** | CONFIRMED | DFS backtracks when there are no *unvisited neighbors*. | "…until it reaches a vertex with no unvisited neighbors, then backtracks." |
| P4-35 | 70 / 69 | "same number of vertices as the graph, but vertices are not equal" | factual | **ERROR** | CONFIRMED | The sentence contradicts itself. "Edges" is meant. | "…the same vertices as the graph, but a different number of **edges**." |
| P4-36 | 70 / 69 | "edges (Spanning tree) = no. of edge (in graph) − 1" | factual | **ERROR** | CONFIRMED | \|E(T)\| = \|V\| − 1. E − 1 gives the right count only because the example is a 5-cycle. | \|V\| − 1 |
| P4-37 | 70 / 69 | tree (i): 2–3 labelled "1" | diagram | **ERROR** | CONFIRMED | Zoomed: edge 2–3 is clearly labelled "1". The circled sum 12 = 1 + 4 + 5 + 2 shows it should be 4. | 4 |
| P4-38 | 70 / 69 | tree (iii): vertex "9" | diagram | **ERROR** | CONFIRMED | Zoomed: the vertex is clearly labelled "9". The edges and the sum 11 = 1 + 3 + 2 + 5 show it is vertex 4. | 4 |
| P4-39 | 70–71 / 69–70 | only 3 + MST of the 5 spanning trees shown | diagram | **IMPRECISE** | CONFIRMED | Recomputed: removing each edge in turn gives sums 14, 12, 11, 10, and 13. There is no tree with sum 13. "Less than above" still holds for the trees shown, so this is minor. | Show all 5 trees or mention the tree of weight 13. MST = 10 is correct. |
| P4-40 | 71 / 70 | "MST is a tree whose sum of edge weights is minimum" | factual | **IMPRECISE** | CONFIRMED | It must be a *spanning* tree with minimum total weight. | MST: a spanning tree of G with minimum total edge weight among all spanning trees of G. |
| P4-41 | 71 / 70 | "having an edge weight i.e. 10" | factual | **IMPRECISE** | CONFIRMED | 10 is the total weight, not the weight of a single edge. MST = {1, 2, 3, 4} = 10 (Kruskal) is correct. | "…the MST is the spanning tree of minimum total weight, here 10." |
| P4-42 | 71 / 70 | "same number of vertices in given graph minus 1" | factual | **ERROR** | CONFIRMED | Literally says the trees have V − 1 vertices. It should be V − 1 edges. | "All spanning trees of G have the same number of **edges**, \|V\| − 1." |
| P4-44 | 71 / 70 | "If we remove one more edge … as." | factual | **IMPRECISE** | CONFIRMED | The sentence is unfinished. | "Removing any edge disconnects a spanning tree; adding any edge creates a cycle." |
| P4-45 | 71 / 70 | "there would be more than two minimum spanning tree" | factual | **ERROR** | CONFIRMED | Equal weights make multiple MSTs *possible*, not mandatory. A graph that is itself a tree has exactly one spanning tree. | "may have more than one MST" |
| P4-46 | 71 / 70 | "distinct weight … only one / unique spanning tree" | factual | **ERROR** | CONFIRMED | Distinct weights make the *minimum* spanning tree unique, not the spanning tree in general. | "a unique MST" |
| P4-47 | 72 / 71 | "Building a network … possibility that it forms a loop." | factual | **IMPRECISE** | CONFIRMED | Incomplete: does not say how the spanning tree is used (loop elimination, as in STP). | "…a spanning tree of the router graph eliminates loops (STP) or gives a minimum-cost connection (MST)." |

<a id="14"></a>

## 14. Searching (PDF 72–79)

| ID | PDF/nb. page | Quote | Category | Severity | Verdict | Why it is wrong | Correct version |
|---|---|---|---|---|---|---|---|
| P4-48 | 72 / 71 | "we simply traverse the list completely" | factual | **IMPRECISE** | CONFIRMED | Linear search stops at the first match. | "…compare elements one by one until a match is found or the end of the list is reached. Worst case O(n), best case O(1)." |
| P5-01 | 73 / 72 | `SET POS =1` | pseudocode bug | **ERROR** | CONFIRMED | On the page this is a plain-stroke "1", unlike the serifed "I" in `SET I`. POS should store the index where the value was found. | `SET POS = I` |
| P5-02 | 73 / 72 | Space row: only worst = `O(1)` | inconsistency | **IMPRECISE** | CONFIRMED | The best- and average-case cells are empty. Space is O(1) in all cases. | O(1) in all three cases |
| P5-03 | 73 (also 77, 81) / 72 | `void main ()` | non-standard C | **ERROR** | CONFIRMED | C11 5.1.2.2.1. Reproduced with clang: `error: 'main' must return 'int'` (vm.c). The same line appears on p. 77 and p. 81. | `int main(void)` |
| P5-04 | 74 / 73 | `scanf("%d", item);` | UB | **ERROR** | CONFIRMED | There is no `&` on the page. scanf receives an indeterminate int instead of an `int *` (C11 7.21.6.2p12). | `scanf("%d", &item);` |
| P5-05 | 74 / 73 | `else flag = 0;` | code quality | **IMPRECISE** | CWC | The issue is valid: the code is fragile but works. The original entry is wrong to say "the loop always runs 10 times", since `break` can end it early. Correct reasoning: flag is written on the very first iteration either way. | Initialize `flag = 0` before the loop |
| P5-06 | 75 / 74 | "works on efficiently on sorted lists" | factual | **IMPRECISE** | CONFIRMED | Binary search is correct only on sorted data. The page's next sentence says exactly that. | "works only on sorted lists" |
| P5-07 | 75, 78 / 74, 77 | `MID = (BEG + END)/2`, `mid=(beg +end)/2;` | overflow | **IMPRECISE** | CONFIRMED | `beg+end` can overflow a signed int (C11 6.5p5). At these sizes it will not happen. | `beg + (end-beg)/2` |
| P5-08 | 76 / 75 | "Worst case space complexity O(1)" | complexity | **IMPRECISE** | CWC | The table describes the **iterative** pseudocode on p. 75, and for it the table is correct. Only the later recursive C program (pp. 77–79) uses O(log n) stack. So this is a caveat about the C code, not an error in the table. | Iterative: O(1); recursive variant: O(log n) |
| P5-10 | 77 vs 78–79 / 76–78 | "location of item will be 7" vs `return mid+1;` | inconsistency | **IMPRECISE** | CONFIRMED | Retraced: MID goes 4→6→7, a[7] = 23, so the 0-based answer is 7. The C code returns 1-based positions, while the pseudocode on p. 75 returns MID. | State the indexing convention explicitly |
| P5-11 | 78–79 / 77–78 | `int Binary search (int a[], ...)`, `return binary search (a, beg, mid-1, item);` | compile error | **ERROR** | CONFIRMED | Both spots are visible on the page. Identifiers cannot contain spaces and are case-sensitive, so the `binarysearch` prototype has no definition. | `binarysearch` everywhere |

<a id="15"></a>

## 15. Sorting (PDF 79–93)

| ID | PDF/nb. page | Quote | Category | Severity | Verdict | Why it is wrong | Correct version |
|---|---|---|---|---|---|---|---|
| P5-N1 | 79 / 78 | "swapping of adjacent elements until they are not in intended order" | factual (inverted) | **IMPRECISE** | NEW | Bubble sort swaps adjacent elements **if** they are not in the intended order, and repeats **until** they are in order. "until ... not in intended order" says the opposite. | "…swapping adjacent elements if they are in the wrong order" |
| P5-13 | 80 / 79 | `for all array elements / if arr[i] > arr[i+1] swap` | pseudocode bug | **ERROR** | CONFIRMED | This is a single pass with no outer loop, so the array is not sorted. Also, `arr[i+1]` goes out of bounds on the last i. | Nested passes, optionally with a `swapped` flag |
| P5-14 | 80 / 79 | "Best case O(n)" | complexity | **IMPRECISE** | CONFIRMED | O(n) requires an early-exit flag. Neither the pseudocode nor the C code has one, so as written the best case is Θ(n²). | "O(n) with a swapped flag" |
| P5-18 | 80/82 (also 85, 90) / 79/81 | `printf("%d", a[i]);` vs spaced output | inconsistency | **ERROR** | CONFIRMED | No separator is printed. Reproduced: the bucket sort program prints `54128457694195` (bk.c). The shown output does not match what the code prints. | `"%d "` |
| P5-15 | 81 / 80 | `for(j=i+1;...) if(a[j] < a[i]) swap a[i],a[j]` | terminology | **ERROR** | CONFIRMED | This is exchange sort: it swaps non-adjacent elements. Contradicts the notes' own definition of bubble sort on p. 79. The result is nevertheless sorted correctly. | Bubble sort with adjacent swaps |
| P5-16 | 81 / 80 | `sizeof(a) / sizeof(a0)` | compile error | **ERROR** | CONFIRMED | The page clearly has `(a0)` — an undeclared identifier. | `sizeof(a[0])` |
| P5-17 | 81 / 80 | `int i, j, temp;` in main | code quality | **IMPRECISE** | CONFIRMED | The variables are unused. | Remove them |
| P5-19 | 82 / 81 | "not useful if we have a large array" | factual | **IMPRECISE** | CONFIRMED | Bucket sort degrades because of non-uniform distribution or a wide key range, not size (CLRS §8.4). | "Not suitable when the input is non-uniformly distributed / the key range is large." |
| P5-20 | 82 vs 84 / 81/83 | "may or may not be stable" vs "stable: YES" | inconsistency | **ERROR** | CONFIRMED | The two statements on these pages contradict each other. | "Stable if the sort within buckets is stable" |
| P5-21 | 82 / 81 | "reduces no. of comparisons", "asymptotically fast" | factual | **IMPRECISE** | CONFIRMED | Both claims hold only for uniformly distributed input. | Add this condition |
| P5-22 | 83 / 82 | Step 1 `let B[0....n-1]` before step 2 `n = length[A]` | inconsistency | **IMPRECISE** | CWC | n is indeed used before it is defined. But the original fix "swap the steps (as in CLRS)" misstates the source: CLRS 3e BUCKET-SORT has exactly this order (line 1 `let B[0..n−1]`, line 2 `n = A.length`). The notes copied it faithfully. | Swap the steps for clarity (a CLRS quirk, not a deviation from it) |
| P5-23 | 83 / 82 | `insert A[i] into list B[n a[i]]` | pseudocode bug | **ERROR** | CONFIRMED | The floor is missing (CLRS: B[⌊nA[i]⌋]), the input-in-[0,1) assumption is not stated, and `A`/`a` are mixed. | `B[⌊n·A[i]⌋]` |
| P5-24 | 83/84 / 82/83 | O(n+k) with k undefined | terminology | **IMPRECISE** | CONFIRMED | k is never defined. | k = number of buckets |
| P5-25 | 84 / 83 | "Space complexity O(n*k)" | complexity | **ERROR** | CONFIRMED | The page clearly says `O(n*k)`. The correct bound is O(n+k). | O(n+k) |
| P5-26 | 84–85 / 83–84 | `bucket[a[i]]++ ... while(bucket[i]>0) a[j++]=i;` | terminology | **ERROR** | CONFIRMED | This is counting sort, not bucket sort. Negative keys index out of bounds. | Call it counting sort |
| P5-28 | 84 / 83 | `int max = getmax (a, n)` | compile error | **ERROR** | CONFIRMED | Zooming confirms the missing semicolon. | add `;` |
| P5-29 | 84–85 / 83–84 | `int bucket[max]` + `i<=max`, `bucket[a[i]]++` | UB (OOB) | **ERROR** | CONFIRMED | Reproduced: UBSan `index 84 out of bounds for type 'int[max]'` and ASan `dynamic-stack-buffer-overflow` (bk.c). | `int bucket[max+1]` |
| P5-30 | 85 / 84 | `for (int i = 0 ; j=0 ; i<=max ;i++)` | compile error | **ERROR** | CONFIRMED | The loop header on the page has four sections, and `j` is undeclared. | `for (int i=0, j=0; i<=max; i++)` |
| P5-31 | 85 / 84 | `sizeof a[0]);` | compile error | **IMPRECISE** | CONFIRMED | `(` is missing before `a`; `[` is written over something. The parentheses are unbalanced. | `sizeof(a[0])` |
| P5-32 | 86 / 85 | "A heap is a complete binary tree" | factual | **IMPRECISE (downgraded)** | CWC | Valid: the heap-order property is missing. Severity downgraded to IMPRECISE: the definition is incomplete rather than false, and the page goes on to use max-heaps. | "…that satisfies the heap property" |
| P5-33 | 86 / 85 | "creating min-heap or max-heap" | factual | **IMPRECISE** | CONFIRMED | An in-place ascending sort uses a max-heap. | Ascending: max-heap. |
| P5-34 | 86 / 85 | `heap-size [arr] = heap-size [arr] ? 1` | typo | **ERROR** | CONFIRMED | The `?` is visible on the page. It is an encoding artifact of a "−" copied from a web source. | `− 1` |
| P5-35 | 86/87 / 85/86 | `for i = length (arr) to 2`, `length(arr)/2 to 1` | terminology | **IMPRECISE** | CONFIRMED | The loops count down and need ⌊·⌋ (CLRS). | `downto`, ⌊length/2⌋ |
| P5-N2 | 86 / 85 | "Heap sort basically recursively performs two main operations" | factual | **IMPRECISE** | NEW | Both phases (build the heap, then repeatedly extract) are iterative loops. Only heapify may be recursive. | "Heap sort runs in two phases" |
| P5-36 | 87 / 86 | `MaxHeapify (arr, i)` outdented | layout | **IMPRECISE** | CONFIRMED | On the page the line sits at the indentation level of the `BuildMaxHeap` line, outside the for loop. | Make it the (indented) loop body |
| P5-37 | 87 / 86 | Best O(n log n), space O(1) | complexity | **IMPRECISE** | CONFIRMED | If all keys are equal and the comparison is a strict `>`, heapify returns immediately, so the running time is O(n). Recursive heapify uses O(log n) stack. | Best Θ(n log n) for distinct keys; space O(1) for iterative / O(log n) for recursive heapify. |
| P5-38 | 87 / 86 | `int largest = i` / `left = 2 * i+1` / `right = 2 * i+2` | compile error | **ERROR** | CONFIRMED | At 220 dpi there are no semicolons. | add `;` |
| P5-39 | 87–88 / 86–87 | `if(left <n && a[left] > a[largest]` | compile error | **ERROR** | CONFIRMED | Both ifs end at `]` with no `)`. | add `)` |
| P5-40 | 88 / 87 | `if (largest != 1)` | infinite recursion | **ERROR** | CONFIRMED | It is a "1": the author dots every `i`, and `a[i]` on the next line has a dot. Reproduced: `!= 1` → exit code 139 (SIGSEGV); `!= i` → `1 10 23 26 28 43 48` (heap.c). | `!= i` |
| P5-41 | 88 / 87 | `i>=0, i--` / `i>0 , i--` | compile error | **ERROR** | CONFIRMED | The commas are visible in both for headers. | `;` |
| P5-42 | 88/89 / 87/88 | `heapsort` defined, `heapSort (a, n);` called | compile error | **ERROR** | CONFIRMED | The capital S is visible on p. 89. | One name |
| P5-44 | 89 / 88 | "It is assumed that the first card." | incomplete | **IMPRECISE** | CONFIRMED | The sentence is cut off. | "…is already sorted" |
| P5-45 | 89 / 88 | "less efficient than ... heap, quick, merge sort" | factual | **IMPRECISE** | CONFIRMED | True only for large random inputs. | state the condition |
| P5-46 | 90 / 89 | `for (i= 1; i<n , i++)` | compile error | **ERROR** | CONFIRMED | The comma is visible. | `;` |
| P5-47 | 90 / 89 | `while (j>=0 && temp <= a[j])` | stability / complexity | **IMPRECISE** | CONFIRMED | `<=` makes the sort unstable. Reproduced: an input of n = 1000 equal elements takes 499,500 comparisons, Θ(n²) (ins.c), contradicting the stated O(n) best case. | `a[j] > temp` |
| P6-01 | 91 / 90 | "beg is starting element and end is last element" | terminology | **IMPRECISE** | CONFIRMED | beg and end are indices. | "index of the first/last element" |
| P6-02 | 91 / 90 | `Set mid = (beg + end)/2` | overflow | **IMPRECISE** | CONFIRMED | Same as P5-07. | `beg+(end-beg)/2` |
| P6-03 | 91–93 / 90–92 | "implementation of merge sort" gives only `merge()` | omission | **IMPRECISE (downgraded)** | CWC | Confirmed that there is neither `mergeSort()` nor `main`. Severity downgraded to IMPRECISE: nothing that is written is wrong; the implementation is just incomplete. | Add `mergeSort` and a driver |
| P6-N1 | 91 / 90 | `int n = sizeof (a) / sizeof a[0]);` | compile error | **ERROR** | NEW | `main` of insertion sort (end of the program from p. 90). `(` is missing before `a[0]`, as in P5-31, so the parentheses are unbalanced. The transcript silently "fixed" this to `sizeof(a[0])`. | `sizeof(a[0])` |
| P6-04 | 92 / 91 | `int LeftArray[n1], RightArray[n2];` | portability | **IMPRECISE** | CONFIRMED | VLAs are optional in C11 (`__STDC_NO_VLA__`) and use stack space. | a malloc'd buffer |
| P6-05 | 92 / 91 | `for (int i = 0; ...)` after `int i, j, k;` | style | **IMPRECISE** | CONFIRMED | The loop variables shadow the outer ones. Legal. | Use the existing i, j |

<a id="16"></a>

## 16. Interview Q1–Q50 (PDF 99–109, notebook 93–103)

| ID | PDF/nb. page | Quote | Category | Severity | Verdict | Why it is wrong | Correct version |
|---|---|---|---|---|---|---|---|
| P6-32 | 99 / 93 | Q3 "Rdbms = array (array of structures)" | factual | **IMPRECISE** | CONFIRMED | A stock answer at the logical level. Real DBMS storage and indexing use B+ trees, heap pages, and hash indexes. | "Relations are modeled as tables (arrays of records); physical storage and indexes are B+ trees or hash structures." |
| P6-33 | 99–100 / 93–94 | Q4 "not possible to use ordinary pointer ... void pointer" | factual | **IMPRECISE** | CONFIRMED | Links can be typed as `struct node *`. Only the payload needs `void *` or a union. | "`struct node *next` for links and `void *data` (plus a type tag) or a union for heterogeneous data." |
| P6-34 | 100 / 94 | Q5 "two" queues | factual | **IMPRECISE** | CONFIRMED | A folklore answer. A heap needs no queue at all. | "No queues are needed (a heap suffices). With FIFO queues, one queue per priority level can be used." |
| P6-35 | 100 / 94 | Q9 "methods available in storing sequential files" | terminology | **ERROR** | CONFIRMED | The listed answers are external **sorting** methods. | "sorting sequential files" |
| P6-36 | 101 / 95 | Q10 "according to storage linked list is a non linear one" | factual | **IMPRECISE** | CONFIRMED | Non-contiguous storage does not make a structure non-linear. Q43 calls the linked list linear. | "A linked list is linear; its nodes are just not stored contiguously." |
| P6-37 | 101 / 95 | Q11 stray empty "•" | typo | **IMPRECISE** | CONFIRMED | An empty bullet is visible. | — |
| P6-38 | 101 / 95 | Q12 "object oriented analysis and design" | factual | **ERROR** | CONFIRMED | Unrelated to sorting efficiency. | Remove |
| P6-39 | 101–102 / 95–96 | Q13 "number of comparisons in any case is O(n)" | complexity | **IMPRECISE** | CONFIRMED | Formally correct as an upper bound, but hides the Θ(1) best case. Reasonably remains IMPRECISE. | Best Θ(1), average/worst Θ(n) |
| P6-40 | 102 / 96 | Q15 "parenthesis is not required" | factual | **IMPRECISE** | CONFIRMED | Holds only when every operator has a fixed arity. | Add "(given fixed operator arity)". |
| P6-41 | 103 / 97 | Q19 "'pivotal value' or height factor" | terminology | **IMPRECISE** | CONFIRMED | The standard term is balance factor, and rebalancing happens at ±2. | balance factor = h(left) − h(right); rebalance at \|BF\| = 2 |
| P6-42 | 103 / 97 | Q20 B+ tree reason | factual | **IMPRECISE** | CONFIRMED | The high fan-out and linked leaves are missing. | Mention the high fan-out and linked leaves for range queries. |
| P6-43 | 103–104 / 97–98 | Q22 `while (pointer1) {...}` | code bug / UB | **ERROR** | CONFIRMED | Retraced. Non-circular list: on the next iteration `pointer2` is NULL and `pointer2->next` is dereferenced. For a 1-node list NULL == NULL, so "circular" is printed falsely. There is no `break`, and `print` is not a C function (written as `print("circularn")`). | Check for a return to the head, or Floyd with `fast && fast->next` |
| P6-44 | 104 / 98 | Q23 "A node class is class that, relies on the base..." | context | **IMPRECISE** | CWC | It is valid that the answer does not fit a DSA interview. Two corrections. (1) The text is an accurate, if ungrammatical, rendering of Stroustrup's four properties of a node class, so in its C++ sense it is correct, not "almost meaningless". (2) The reference is wrong: the passage is in TC++PL 3rd ed. §25.4 "Node Classes" (2nd ed. §13.4), not §12.4.2. | Give the DSA meaning (data and link fields) and mention the C++ class-design meaning |
| P6-45 | 104 / 98 | Q25 "open addressing (closed hashing) ... overflow block" | factual | **ERROR** | CONFIRMED | Open addressing works by probing (linear, quadratic, double hashing; CLRS §11.4). | "Open addressing (closed hashing): linear probing, quadratic probing, double hashing. Closed addressing (open hashing / chaining): linked list, balanced tree." |
| P6-46 | 105 / 99 | Q27, Q28 duplicate Q7, Q16 | inconsistency | **IMPRECISE** | CONFIRMED | Exact duplicates of Q7 and Q16, so of the "50 questions" only 48 are distinct. | — |
| P6-47 | 105 / 99 | Q29 malloc/calloc | factual | **IMPRECISE** | CONFIRMED | Does not mention that malloc leaves memory uninitialized and that calloc takes (nmemb, size). | Add: "malloc leaves memory uninitialized; calloc takes (count, size) and zeroes the memory." |
| P6-48 | 105 / 99 | Q30 "linkedlist.cpp" | factual | **IMPRECISE** | CONFIRMED | This is a convention, not a rule. Members of class templates must be in the header. | "By convention, in the implementation file (e.g. LinkedList.cpp); for templates, in the header." |
| P6-49 | 105 / 99 | Q31 `front = (front + 1) % size` | factual | **IMPRECISE** | CONFIRMED | This is how front advances on dequeue in an array-based circular queue, not a general formula. | "In an array-based circular queue, on dequeue: front = (front + 1) % capacity." |
| P6-50 | 106 / 100 | Q35 "isempty() checks if stack has at least one element" | factual | **IMPRECISE** | CONFIRMED | The wording is inverted: isempty() returns true when there are no elements. | "isempty() returns true if the stack contains no elements (top == −1)." |
| P6-51 | 106 / 100 | Q37 "push is the direction" | terminology | **IMPRECISE** | CONFIRMED | Push is an operation, not a direction. | "push() adds an element to the top of the stack." |
| P6-52 | 107 / 101 | Q39 "type qualifier like signed/unsigned" | terminology | **ERROR** | CONFIRMED | `signed` and `unsigned` are type specifiers (C11 6.7.2). The qualifiers are const, volatile, restrict, and _Atomic (6.7.3). | "Type specifier(s) + declarator (identifier); optionally storage-class specifiers, type qualifiers (const/volatile), and an initializer." |
| P6-53 | 107 / 101 | Q40 "arrays, pointers, structures" | factual | **IMPRECISE** | CONFIRMED | A pointer is not a data structure. | "arrays, (fixed-size) structures/records, stacks or queues on a fixed-size array" |
| P6-54 | 107 / 101 | Q42 "linked lists, stack, queues, trees" | factual | **IMPRECISE** | CONFIRMED | Stacks and queues are dynamic only in the linked implementation. The array-based stack on p. 97 is static. | "Linked lists, linked stacks/queues, trees, graphs (when built with dynamic allocation)." |
| P6-55 | 108 / 102 | Q44 "hierarchical ... eg: trees and graphs" | factual | **IMPRECISE** | CONFIRMED | General graphs are not hierarchical. | "…have hierarchical (trees) or many-to-many network (graphs) relationships" |
| P6-56 | 108 / 102 | Q45 list of types | incomplete | **IMPRECISE** | CONFIRMED | The circular doubly linked list is missing. | Add "circular doubly linked list". |
| P6-57 | 108 / 102 | Q46 "Insertion of a list", "traversal of a node" | wording | **IMPRECISE (downgraded)** | CWC | Confirmed on the page. Severity downgraded from ERROR to IMPRECISE: the wrong nouns do not change which operations are meant. "Searching" is also missing. | "insertion of a node", "traversal of the list", add searching |
| P6-58 | 109 / 103 | Q50 "until a match occurs" | incomplete | **IMPRECISE** | CONFIRMED | Does not mention stopping at the end of the array on an unsuccessful search. | "…or the end is reached" |

<a id="17"></a>

## 17. Coding problems (PDF 94–98, notebook 104–108)

| ID | PDF/nb. page | Quote | Category | Severity | Verdict | Why it is wrong | Correct version |
|---|---|---|---|---|---|---|---|
| P6-06 | 94 / 104 | `#include <windows.h>` | portability | **ERROR** | CONFIRMED | The header is unused and does not exist outside Windows. | Remove |
| P6-07 | 94 / 104 | `char *argv[]}` | compile error | **ERROR** | CONFIRMED | The `}` is visible. | `)` |
| P6-08 | 94 / 104 | `< 3000` → gcount reported as "more than 3000" | logic | **ERROR** | CONFIRMED | A salary of exactly 3000 is counted as "more than 3000". | "≥ 3000" |
| P6-09 | 94 / 104 | `"There are {%d} employee..."` | typo | **IMPRECISE** | CONFIRMED | The braces are printed literally, and the string literals are split across lines. | `"There are %d employees ..."` on one line. |
| P6-10 | 94 / 104 | `scanf ("%d", &salary [i]);` unchecked | robustness | **IMPRECISE** | CONFIRMED | On invalid input salary[i] stays indeterminate and is read later. | Check that the return value == 1 |
| P6-11 | 94–95 / 104–105 | `getchar();` after scanf | code bug | **IMPRECISE** | CONFIRMED | It consumes the leftover `'\n'`, so the program does not pause. | Drain the input line first |
| P6-N2 | 94 / 104 | `if (salary [i] < 3000]` | compile error | **ERROR** | NEW | At 220 dpi the condition is closed with `]`, not `)`. The transcript silently corrected it. | `if (salary[i] < 3000)` |
| P6-12 | 95 / 105 | `using namespace std;` with no include | compile error | **ERROR** | CONFIRMED | No header is included, so `std`, `cout`, `endl`, and `NULL` are undeclared. | `#include <iostream>` |
| P6-13 | 95 / 105 | `class node { ... Node *next; ... Node (T value)` | compile error | **ERROR** | CONFIRMED | Lowercase `node` in the class header and capitalized `Node` in the members — both clearly visible. | `class Node` |
| P6-14 | 95 / 105 | `Node (T value) { this->value = value; }` | latent UB | **ERROR** | CWC | It is valid that next/previous stay uninitialized. Correction: this program never reads the last node's `next` (there is no traversal), so the UB is latent rather than triggered. Combined with P6-18 it becomes reachable as soon as a traversal is added. | Initialize to `nullptr` |
| P6-15 | 95 / 105 | `class Linked list` / `Linked list ()` | compile error | **ERROR** | CONFIRMED | The name contains a space, and main uses a different spelling. | `LinkedList` |
| P6-16 | 95 / 105 | `public ;` | compile error | **ERROR** | CONFIRMED | The semicolon is visible. | `public:` |
| P6-17 | 95–96 / 105–106 | `int size;` vs `this->size_`; `head_` vs `this->head`; `this->tail`; `new node<T>` | compile error | **ERROR** | CONFIRMED | All of these are visible on the pages. | Consistent names |
| P6-24 | 95–96 / 105–106 | No destructor | leak | **ERROR** | CONFIRMED | Every `new Node` leaks. The class also violates the rule of three (copying will double-free once a destructor is added). | Destructor plus the rule of three |
| P6-N4 | 95 / 105 | `this->value = Value;` | possible compile error | **IMPRECISE** | NEW (low confidence) | The "V" on the right is noticeably larger than the "v" in `this->value`. If it is a capital, `Value` is undeclared. The handwriting is ambiguous. | `this->value = value;` (better: a member initializer list) |
| P6-18 | 96 / 106 | `this->tail_->next = new node<T>(value); ...previous = ...` (tail_ not advanced) | code bug | **ERROR** | CONFIRMED | Retraced: append(10), (3), (1) leaves the list 10→1, and node 3 leaks. | `tail_ = tail_->next;` |
| P6-19 | 96 / 106 | `void prepend (T value) {` with no body | compile error | **ERROR** | CONFIRMED | Only the opening brace is visible. | Implement it |
| P6-20 | 96 / 106 | `resetIterator () { tail_ = NULL; }` | code bug | **ERROR** | CONFIRMED | Retraced: the next append takes the else branch and dereferences a NULL `tail_`. | `itr = head_;` |
| P6-21 | 96 / 106 | Class never closed | compile error | **ERROR** | CONFIRMED | `main` follows immediately after the `}` of `resetIterator`. | `};` |
| P6-22 | 96 / 106 | `cout << "printing linked list << endl;` | compile error | **ERROR** | CONFIRMED | The closing quote is missing. | Close the string |
| P6-23 | 96 / 106 | List never printed | inconsistency | **ERROR** | CONFIRMED | "printing linked list" is printed, but there is no traversal, iterator, or print method. | Add a traversal, e.g. `for(auto p=head_; p; p=p->next) cout<<p->value<<' ';` |
| P6-N3 | 96 / 106 | `LinkedList <int> lList ;` then `llist.append (10);` ×3 | compile error | **ERROR** | NEW | `lList` (capital L) is declared but `llist` (lowercase) is used. Identifiers are case-sensitive, so `llist` is undeclared. The transcript normalized it to `lList`. | One spelling |
| P6-25 | 97 / 107 | `if (top == MAXSIZE)` | off-by-one / UB | **ERROR** | CONFIRMED | Reproduced (st.c): top reaches 8, and UBSan reports `index 8 out of bounds for type 'int[8]'`. "Full" is reported only on the 10th push. | `top == MAXSIZE-1` |
| P6-26 | 97 / 107 | `int MAXSIZE = 8; int stack [8];` | inconsistency | **IMPRECISE** | CWC | The duplicated capacity is confirmed. Correction to the original entry: in C even a `const int` cannot size a file-scope array, because it is not an integer constant expression (C11 6.6p6). So the wording "non-const int cannot..." implies that const would work, which it does not. | `#define MAXSIZE 8` or `enum { MAXSIZE = 8 };` |
| P6-27 | 97 / 107 | `int pop() { ... else { printf(...); } }` | latent UB | **ERROR** | CWC | The clang `-Wreturn-type` warning is reproduced. Correction: in this `main`, pop() is called only when `!isempty()`, so the path without a return is never taken here. The UB (C11 6.9.1p12) is latent, not caused by `int data = pop();` as the original entry claims. | Return a sentinel value or use an out-parameter |
| P6-28 | 97–98 / 107–108 | `int push(int data)` with no return | code bug | **IMPRECISE** | CONFIRMED | The warning is reproduced. Not UB, since the value is not used. | `void push` |
| P6-29 | 97 / 107 | `int peek() { return stack[top]; }` | latent UB | **IMPRECISE** | CONFIRMED | Reads stack[-1] when the stack is empty. Not triggered here. | Check isempty |
| P6-30 | 97 / 107 | `int isempty ()` etc. | style | **IMPRECISE** | CONFIRMED | `()` is a non-prototype declarator in C ≤ C17. | `(void)` |
| P6-31 | 98 / 108 | `isempty() "true", "false"` | compile error | **ERROR** | CONFIRMED | Visible on the page. The corrected program was retraced: pop prints 15 12 1 9 5 3. | `isempty() ? "true" : "false"` |

<a id="rejected"></a>

## Rejected candidates (not errors)

These candidates were in the transcribers' initial lists, but verification showed there is no error. The "Claimed severity" column is the rating from the initial list.

| ID | PDF/nb. page | Chapter | Quote | Category | Claimed severity | Reason for rejection | Conclusion |
|---|---|---|---|---|---|---|---|
| P2-27 | 27 / 26 | 05 | base address 999 with 2-byte elements | alignment | IMPRECISE | A standard paper exercise in address arithmetic. Nothing on the page claims a real machine; byte-addressed models without alignment are the norm in such exercises (some ISAs allow unaligned access). Not an error. | — |
| P2-28 | 28 / 27 | 05 | cell `a[n-?][2]` | diagram / typo | IMPRECISE | On the page a digit is overwritten by hand — a strike-over correction, not a content error. The row label "n-1" and the neighboring cells unambiguously give `a[n-1][2]`. This belongs in the transcription notes, not in the errata. | (transcription: `a[n-1][2]`, digit overwritten) |
| P3-30 | 47 / 46 | 09 | "same priority … served according to FIFO principle" | factual | IMPRECISE | This is the textbook definition in the tradition the notes follow. Lipschutz (Schaum's DS): "two elements with the same priority are processed according to the order in which they were added to the queue". Heap-based priority queues are not stable, but the ADT definition here is a legitimate convention, not an error. At most, add a note that a heap does not guarantee this. | (optional note only) |
| P4-15 | 62 / 61 | 11 | "takes O(log n) time" | complexity | IMPRECISE | For a fixed order m (the usual assumption), both the CPU cost O(m·log_m n) and the disk cost O(log_m n) are O(log n). CLRS 18.2 gives O(t log_t n), which is O(log n) for constant t. The statement is correct; the original entry merely refines it. | (no change needed) |
| P4-18 | 63 / 62 | 11 | "leaf nodes … linked … singly linked list" | factual | IMPRECISE | A singly linked chain of leaves is the classic description of a B+ tree (e.g. Silberschatz, Ramakrishnan). Doubly linked leaves are an implementation variant. The sentence does not claim exclusivity, so it is not an error. | (no change needed) |
| P5-09 | 76 / 75 | 14 | `END = 8 ron` | typo | IMPRECISE | END = 8 = n−1 is correct for a 9-element array. The squiggle after 8 is illegible ("8 (n)"?) and does not claim n = 9. Nothing here is demonstrably wrong. | — |
| P5-12 | 78 / 77 | 14 | `scanf(" %d ", &item);` | code bug | IMPRECISE | The claim cannot be verified. The gaps near the quotes are ordinary spacing in the author's handwriting; the same gap appears in `printf(" Enter ...`. A trailing space inside the format cannot be reliably read from the manuscript. The technical claim would be correct *if* the space were there. | (assume `"%d"`) |
| P5-27 | 84 / 83 | 15 | `masu = a[i];` | compile error | ERROR | **Misreading.** At 220 dpi the word is `max`, written with the author's looped "x"; the same letter form appears in `int max` two lines above. The getmax function is correct, and the claim "max is never updated" is false. | (no error) |
| P5-43 | 88 / 87 | 15 | `printf("%d ", arr[i]); printf(" ");` | inconsistency | IMPRECISE | **Misreading.** The page has `printf("%d", arr[i]);` with no space inside the quotes, followed by `printf(" ");`. This prints exactly one space after each number and matches the shown output. It is the *correct* printArr in this part. | (no error) |

<a id="summary"></a>

## Summary

Number of items per chapter (excluding rejected).

| No. | Chapter | ERROR | IMPRECISE | Total |
|---|---|---:|---:|---:|
| 01 | Intro & DS classification | 6 | 9 | 15 |
| 02 | Algorithms & asymptotic analysis | 8 | 15 | 23 |
| 03 | Pointers | 4 | 5 | 9 |
| 04 | Structures | 4 | 2 | 6 |
| 05 | Arrays | 8 | 12 | 20 |
| 06 | Linked lists | 3 | 8 | 11 |
| 07 | Skip list | 7 | 11 | 18 |
| 08 | Stack | 4 | 4 | 8 |
| 09 | Queue | 3 | 3 | 6 |
| 10 | Trees | 9 | 14 | 23 |
| 11 | Tree types: BST/AVL/B/B+ | 3 | 8 | 11 |
| 12 | Graphs | 3 | 7 | 10 |
| 13 | Graph traversals / spanning trees | 8 | 10 | 18 |
| 14 | Searching | 4 | 7 | 11 |
| 15 | Sorting | 19 | 22 | 41 |
| 16 | Interview Q1–Q50 | 5 | 22 | 27 |
| 17 | Coding problems | 21 | 8 | 29 |
| | **Total** | **119** | **167** | **286** |

Breakdown by verdict (excluding rejected): CONFIRMED — 240, CWC (confirmed with correction) — 31, NEW — 15.

Rejected candidates: 9 (P2-27, P2-28, P3-30, P4-15, P4-18, P5-09, P5-12, P5-27, P5-43).
