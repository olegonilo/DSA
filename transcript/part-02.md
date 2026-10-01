# DSA Handwritten Notes (Topper World) — Part 02: PDF pages 19–36

> Verbatim transcription of the handwritten DSA notebook (English, as written), PDF pages 19–36.
> Original errors are intentionally preserved; only transcription mistakes were fixed (each marked with `<!-- corrected: ... -->`).
> For the list of errors in the notes themselves see [../errata/ERRATA-full.md](../errata/ERRATA-full.md).


### PDF p.19 (notebook p.18)

**Pointer :-**
Pointer is used to points the address of the value stored anywhere in the computer memory. To obtain the value stored at location is known as dereferencing pointer.

**Pointer arithmatic :-**
4 arithmatic operators that can be used in pointers : `++`, `--`, `+`, `-`

**Array of pointers :-** You can define array of to hold a number of pointers.

**Pointer to pointer :-** C allows you to have pointer on a pointer and so on.

Diagram:
- `a →` box containing `10` (→ "value"); under it `2000` (→ "address").
- `b →` empty box; under it `3000`.
- `b = &a →` empty box at `3000`, with an arrow from it to an empty box at `2000`; annotation `[b points a]`.

**Program**
Pointer →

```c
#include <stdio.h>
int main ()
```

### PDF p.20 (notebook p.19)

```c
{
int a = 5;
int *b;
b = &a;
printf ("value of a = %d \n", a);
printf ("value of a = %d \n", *(&a));
printf ("value of a = %d \n", *b);
printf ("address of a = %u \n", &a);
printf ("address of a = %d \n", b);
printf ("address of b = %u\n", &b)
printf ("value of b = address of a = %u", b);
return 0;
}
```

output →
```
value of a = 5
value of a = 5
address of a = 3010494292
address of a = -1284473004
address of b = 3010494296
value of b = address of a = 3010494292.
```
(Note: only 5 output lines are written for 7 printf calls — the third "value of a = 5" line is absent.)

**Program :-**
Pointer to pointer :-

```c
#include <stdio.h>
int main ()
{
int a = 5;
int *b;
int **c;
```

### PDF p.21 (notebook p.20)

```c
b = &a;
c = &b;
printf ("value of a = %d \n", a);
printf ("value of b = address of a = %u \n", b);
printf ("value of c = address of b = %u \n", c);
printf ("address of b = %u \n", c);
printf ("address of c = %u \n", &c);
return 0;
}
```

output →
```
value of a = 5
value of b = address of a = 2831685116
value of c = address of b = 2831685120
address of b = 2831685120
address of c = 2831685128.
```

**Structure :-**
A structure is a composite data type that defines a grouped list of variables that are to be placed under one name in block of memory.

**Program :-**
structure →

```c
struct structure_name
{
data-type member 1;
data-type member 2;
```

### PDF p.22 (notebook p.21)

```c
    .
    .
    .
data-type member ;
};
```

**Advantages of structure :-**
- It can hold variables of different data types.
- We can create objects containing different types of attributes.
- It allows us to re-use the data layout across programs.
- It is used to implement other data structure like linked list, queues, trees and graphs.

**Program :-**
how to use structure in program →

```c
#include <stdio.h>
#include <conio.h>
void main ()
{
struct employee
{
int id;
float salary;
int mobile;
};
```

### PDF p.23 (notebook p.22)

```c
struct employee e1, e2, e3;
printf ("\n Enter ids, salary & mobile no. \n");
scanf (" %d %f %d", &e1.id, &e1.salary, &e1.mobile);
scanf ("%d %f %d ", &e2.id, &e2.salary, &e2.mobile);
printf (%d %f %d", &e3.id, &e3.salary, &e3.mobile);
printf ("\n Entered result ");
printf ("\n %d %f %d", e1.id, e1.salary, e1.mobile);
printf ("\n %d %f %d ", e2.id, e2.salary, e2.mobile);
printf ("\n %d %f %d", e3.id, e3.salary, e3.mobile);
getch ();
}
```

output → guess the output
And write it here....

### PDF p.24 (notebook p.23)

**Array :-** Arrays are defined as collection of similar type of data items stored at contigous memory locations.
Array is the simplest data structure where each data element can be randomly accessed by using its index number.

**Array declaration :-**
`int arr[10]; char arr[10]; float arr[5]`

**Program without Array :-**
```c
#include <stdio.h>
void main ()
{
int marks_1 = 56; marks_2 = 78, marks_3 = 89;
Float avg = (marks_1 + marks_2 + marks_3)/3;
print (avg);
}
```

**Program by using Array :-**
```c
#include <stdio.h>
void main
{
int marks [3] = {56, 78, 89};
int i;
float avg;
for (i=0; i<3; i++)
```

### PDF p.25 (notebook p.24)

```c
{
avg = avg + marks [i];
}
printf (avg);
}
```

**Complexity of Array operations :-**

1). Time complexity :-

| Algorithm | Average case | worst case |
|---|---|---|
| Access | O(1) | O(1) |
| search | O(n) | O(n) |
| insertion | O(n) | O(n) |
| Deletion | O(n) | O(n) |

2). space complexity :- In Array space complexity for worst case is O(n)

**Memory Allocation of the Array :-**
Each element in Array represented by indexing. Indexing of array can be defined in three ways:
1. 0 (zero Based indexing) :- The first element of the array will be arr[0].

### PDF p.26 (notebook p.25)

2. 1 (one-based indexing) :- The first element of array will be arr[1].
3. n (n-based indexing) :- The first element of array can reside at any random index number.

Diagram (fig : int arr[5]): a row of 5 cells `arr[0] | arr[1] | arr[2] | arr[3] | arr[4]` with addresses: 100 (arrow up into arr[0], labelled "Base address"), 104 (arrow down into arr[1]), 108 (arrow up into arr[2]), 112 (arrow down into arr[3]), 116 (arrow up into arr[4]).

**Accessing elements of an Array :-**
To access any random element of an array we need the following information:
1. Base address of the array
2. size of an element in bytes.
3. which type of indexing, array follows.

Address of any element of 1D array can be calculate[d]

Byte address of element A[i] = base address + size * (first - index)

Example: In an array, A[-10 .... +2] Base address (BA) = 999, size of an element = 2 bytes, find location of A[-1].

### PDF p.27 (notebook p.26)

solution: L(A[-1]) = 999 + [(-1) - (-10)] x 2.
= 999 + 18
= 1017.
∴ location of A[-1] = 1017.

**Passing array to the function :-**
The name of the array represents the starting address or the address of the first element of the array.

Program:
```c
#include <stdio.h>
int summation (int []);
void main ()
{
int arr[5] = {0,1,2,3,4};
int sum = summation (arr);
printf ("%d ", sum);
}
int summation (int arr[])
{
int sum = 0, i;
for(i=0 ; i<5 ; i++)
{
sum = sum + arr[i];
}
return sum ;
}
```

### PDF p.28 (notebook p.27)

**2D Array :-** 2D array can be defined as an array of arrays. The 2D array is organized as matrices which can be represented as collection of rows and coloumns.

**How to declare 2D Array :-**
The syntax for declaration of two dimensional array is as follows : <!-- corrected: spelling: the handwriting reads "syntan" here (p.28) and "symtan" on p.27; normalized to "syntax" -->
`int arr[max_rows][max_coloumns];`

However, it produces the data structure which looks like following :

Diagram (fig : a[n][n]), grid with column headers 0, 1, 2, ..., n-1 and row headers 0, 1, 2, ⋮, n-1:

| | 0 | 1 | 2 | ... n-1 |
|---|---|---|---|---|
| 0 | a[0][0] | a[0][1] | a[0][2] | ..... a[0][n-1] |
| 1 | a[1][0] | a[1][1] | a[1][2] | .... a[1][n-1] |
| 2 | a[2][0] | a[2][1] | a[2][2] | ... a[2][n-1] |
| ⋮ | ⋮ | ⋮ | ⋮ | ⋮ |
| n-1 | a[n-1][0] | a[n-1][1] | a[n-1][2] (digit struck over and rewritten; the intended n-1 is certain) | a[n-1][n-1] |
<!-- corrected: the overwritten digit is a strike-correction; a[n-1][2] is certain, not just likely (see P2-28, rejected as an erratum) -->

Under the grid: `a[n][n]`.

**How to access data in 2D-array :-**
Due to fact that elements of 2D arrays can be random accessed.

### PDF p.29 (notebook p.28)

`int x = a[i][j];`
where i, j are the rows and coloumns respectively.

**Initializing 2D arrays :-**
The syntax to declare and initialize the 2D array is given as follows :
`int arr[2][2] = {0,1,2,3};`

number of elements in 2D arrays = number of rows * number of coloumns.

**Mapping 2D array to 1D array :-**
The size of a two dimensional array is equal to the multiplication of number of rows and number of coloumns present in the array.
A 3x3 two dimensional array is a shown:-

```
        0      1      2      <- coloumn index
  0   (0,0)  (0,1)  (0,2)
  1   (1,0)  (1,1)  (1,2)
  2   (2,0)  (2,1)  (2,2)
  ^
  row index
```

There are two main techniques of storing 2D array elements into memory.

### PDF p.30 (notebook p.29)

1. **Row major ordering :-** In row major ordering, all the rows of 2D array are stored into memory contiguously.
   Diagram: 3x3 matrix a11 a12 a13 / a21 a22 a23 / a31 a32 a33 with red arrows a11→a12→a13, diagonal back from a13 to a21, a21→a22→a23, diagonal back from a23 to a31, a31→a32→a33.
2. **Column major ordering :-** According to coloumn major ordering, all the coloumns of 2D array are stored into the memory contigously.
   Diagram: same 3x3 matrix with downward lines a11→a21→a31, diagonal from a31 up to a12, a12→a22→a32, diagonal up to a13, a13→a23→a33 (red down-arrow at end).

**Calculating address of random element of a 2D array :-**
1). By row major order :- If array is declared a[m][n] where m is the number of rows while n is number of coloumns, then address of an element a[i][j] is calculated as,
Address (a[i][j]) = B.A + (i * n + j) * size
B.A → Base Address

2). By coloumn major order :-
Address (a[i][j] = (j * m) + i) * size + B.A. <!-- corrected: note for readers: the parentheses here are balanced but misplaced (the group swallows the = sign); see P2-32 -->

### PDF p.31 (notebook p.30)

**Linked list :-**
✓ why there is a need of linked list?
If we declare an array of size 3. As we know that all the values of an array are stored in a continous manner, so all three values of an array are stored in a sequential fashion.
Then, total memory space occupied by array would be 3 * 4 = 12 bytes.

**Drawbacks of using array :-**
- we cannot insert more than 3 elements in above example because only 3 spaces are allocated by 3 elements.
- In case of array, the wastage of memory can occur.
- In array, we are providing fixed-size at compile time, due to which wastage of memory occurs.
The solution to this problem is to use linked list.

**What is Linked List?**
A linked list is also a collection of elements, but the elements are not stored in a consecutive location. or linked list is a collection of the nodes in which one node is connected to another node and node consists of two parts i.e. one is data part and second one is the address part.

Diagram: `Head` box containing `4800`, arrow to node [10 | 4900] (address 4800) → [15 | 5000] (address 4900) → [5 | 3000] (address 5000) → [20 | null] (address 3000).

### PDF p.32 (notebook p.31)

**declaration of linked list :-**
In linked list, one is variable and second one is pointer variable. We can declare linked list by using user-defined data type called as structure.

```c
struct node
{
  int data;
  struct node *next;
}
```

**Types of linked list :-**
1). **singly linked list :-** The singly linked list is most common. which consists of data part and address part. The address part in the node is known as a pointer.
Example :- suppose we have three nodes and addresses of these three nodes are 100, 200 and 300 :
Diagram: `head` box containing `100` with arrow to [1 | 200] (addr 100) → [2 | 300] (addr 200) → [3 | NULL] (addr 300).

NULL means its address part does not point to any node. The pointer that holds the address of the initial node is known as a head pointer.

### PDF p.33 (notebook p.32)

2. **Doubly linked list :-** As name suggests, the doubly linked list contains two pointers. We define it in three part the data part and the two address part.
Diagram: [NULL | 1 | 200] (addr 100) ⇄ [100 | 2 | 300] (addr 200) ⇄ [200 | 3 | NULL] (addr 300); `head` box containing `100` pointing to first node.

**Representation of doubly linked list :-**
```c
struct node
{
  int data;
  struct node *next;
  struct node *prev;
}
```

3. **Circular linked list :-** A circular linked list is a variation of a singly linked list. The only difference is "last node does not point to any node in a singly linked list".
Diagram: [7 | 200] (addr 100) → [8 | 300] (addr 200) → [10 | 100] (addr 300), with a red line from the last node back to the first node; `head` box containing `100`.

### PDF p.34 (notebook p.33)

**Representation of circular linked list :-**
```c
struct node
{
  int data;
  struct node *next;
}
```

4. **Doubly circular linked list :-** The doubly circular linked list has the features of both the circular linked list and doubly linked list.
Diagram: [300 | 1 | 200] (addr 100), [100 | 2 | 300] (addr 200), [200 | 3 | 100] (addr 300); red lines: from the last node's next back to the first node (top loop) and from the first node's prev to the last node (bottom loop); `head` box containing `100`.

The last node is attached to the first node and thus creates a circle.
The main difference is that doubly circular linked list does not contain NULL value in previous field of the node.

**Representation of doubly circular linked list :-**
```c
struct node
{
  int data;
  struct node *next;
  struct node *prev;
}
```

### PDF p.35 (notebook p.34)

**Complexity :-**

| | Average: Access | search | Insertion | deletion | Space complexity (worst) |
|---|---|---|---|---|---|
| singly linked list | O(n) | O(n) | O(1) | O(1) | O(n) |

| | Worst: Access | search | Insertion | deletion |
|---|---|---|---|---|
| singly linked list | O(n) | O(n) | O(1) | O(1) |

**Operations on singly linked list :-**
1). Node creation :-
```c
struct node
{
    int data;
    struct node *next;
};
struct node *head, *ptr;
ptr = (struct node *) malloc(sizeof(struct node *)
```

2). Insertion :-
1. Insertion at beginning :- It involves inserting any element at the front of the list. We just need a few link adjustment to make new node as head of list.
2. Insertion at end of list :- The new node can be inserted as the only node in the list / it can be inserted as last one.
3. Insertion after specified node :- we need to skip desired number of nodes in order to reach node after which the new node will be inserted.

### PDF p.36 (notebook p.35)

3). 3) Deletion and Traversing :- <!-- corrected: the number is written twice on the page -->
1. Deletion at beginning :- It just needs few adjustments in the node pointers
2. Deletion at end of list :- The list can either be empty or full. Different logic is implemented for different scenario's.

Traversing :- In traversing, we simply visit each node of the list at least once in order to perform some specific operation in it, for example, printing data part of each node present in the list.

Searching :- In searching, we match each element of the list with the given element. If the element is found on any of the location of that element is returned otherwise null is returned.

**Operations on doubly linked list :-**
1). Node creation :-
```c
struct node
{
  struct node *prev;
  int data;
  struct node *next;
};
struct node *head;
```

2). Insertion :-
1. Insertion at beginning :- Adding the node into the linked list at beginning.
2. Insertion at end :- Adding the node into the linked list to the end.
