# DSA Handwritten Notes — Part 05 (PDF pages 73–90)

> Verbatim transcription of the handwritten DSA notebook (English, as written), PDF pages 73–90.
> Original errors are intentionally preserved; only transcription mistakes were fixed (each marked with `<!-- corrected: ... -->`).
> For the list of errors in the notes themselves see [../errata/ERRATA-full.md](../errata/ERRATA-full.md).


### PDF p.73 (notebook p.72)

**Algorithm :-**
LINEAR_SEARCH (A, N, VAL)

```text
step 1 :- [INITIALIZE] SET POS = -1
step 2 :- [INITIALIZE] SET I = 1
step 3 :- Repeat step 4 while I <= N
step 4 :- IF A[I] = VAL
            SET POS = 1
            PRINT POS
            Go to step 6
          [END OF IF]
          SET I = I+1
          [END OF LOOP].
step 5 :- IF POS = -1
            PRINT "VALUE IS NOT PRESENT in ARRAY"
          [END OF IF].
step 6 :- EXIT.
```
(Note: in step 4 the assignment is clearly written "SET POS =1" — a digit 1, unlike the serifed "I" used elsewhere.)

**Complexity of an Algorithm :-**

| Complexity | Best case | Average case | Worst case |
|---|---|---|---|
| Time | O(1) | O(n) | O(n) |
| Space | | | O(1) |

**C program of linear search :-**
```c
#include <stdio.h>
void main ()
{
```

### PDF p.74 (notebook p.73)

```c
int a[10] = { 10, 23, 40, 1, 2, 0, 14, 13, 50, 9};
int item, i, flag;
printf("\n Enter Item which is to be searched \n");
scanf("%d", item);
for (i=0; i<10; i++)
 {
  if (a[i] == item)
   {
   flag = i+1;
   break;
   }
  else
     flag = 0;
 }
 if(flag != 0)
  {
  printf("\n item found at location %d \n", flag);
  }
  else
  {
   printf("\n Item not found \n");
  }
}
```
Output :-
```text
Enter item which is to be searched
20
Item not found
Enter Item which is to be searched
23
Item found at location 2.
```

### PDF p.75 (notebook p.74)

**2). Binary Search :-** (highlighted)
Binary search is a search technique which works on efficiently on sorted lists. Hence in order to search an element into some list by using binary search technique, we must ensure that list is sorted.
Binary search follows divide and conquer approach in which, list is divided into two halves and item is compared with middle element of list.

**Binary search Algorithm :-**
BINARY_SEARCH (A, lower_bound, upper_bound, VAL)
```text
step 1 :- [INITIALIZE] SET BEG = lower_bound
          END = upper_bound , POS = -1.
step 2 :- Repeat steps 3 and 4 while BEG <= END
step 3 :- SET MID = (BEG + END)/2
step 4 :- IF A[MID] = VAL
            SET POS = MID
            PRINT POS   Go to step 6
          ELSE IF A[MID] > VAL
            SET END = MID - 1
          ELSE SET BEG = MID + 1
step 5 :- IF POS = -1
            PRINT "VALUE IS NOT PRESENT IN ARRAY"
step 6 :- EXIT.
```

### PDF p.76 (notebook p.75)

**Complexity :-**

| Sr.No. | Performance | Complexity |
|---|---|---|
| 1). | Worst case | O(log n) |
| 2). | Best case | O(1) |
| 3). | Average case | O(log n) |
| 4). | Worst case space complexity | O(1) |

**Example:** Let us consider an array arr = {1, 5, 7, 8, 13, 19, 20, 23, 29}. Find location of item 23 in the array.
→ In 1st step :-
BEG = 0
END = 8 [illegible: "ron"/"(n)"?]
MID = 4
a[mid] = a[4] = 13 < 23, therefore;
In second step :-
Beg = mid+1 = 5
End = 8
mid = 13/2 = 6
a[mid] = a[6] = 20 < 23, therefore;
In third step :-
beg = mid+1 = 7
End = 8
mid = 15/2 = 7
a[mid] = a[7].

### PDF p.77 (notebook p.76)

a[7] = 23 = item;
therefore, set location = mid;
The location of item will be 7.

Diagram — "item to be searched : 23". Three identical array drawings (Step 1, Step 2, Step 3), each a row of 9 boxes with values `1 5 7 8 13 19 20 23 29` and indices `0 1 2 3 4 5 6 7 8` written beneath in red. (No pointers/arrows are drawn on the boxes; only "Step N →" labels on the left.)

In step 1 : a[mid] = 13
  13 < 23
  beg = mid + 1 = 5
  end = 8
  mid = (beg + end)/2 = 13/2 = 6
In step 2 :- a[mid] = 20
  20 < 23
  beg = mid + 1 = 7
  end = 8
  mid = (beg + end)/2 = 15/2 = 7.
step 3 :- a[mid] = 23
  23 = 23
  loc = mid.

**C program : Binary search**
```c
#include <stdio.h>
int binarysearch (int [], int, int, int);
void main ()
```

### PDF p.78 (notebook p.77)

```c
{
 int arr[10] = {16, 19, 20, 23, 45, 56, 78, 90, 96, 100};
 int item, location = -1;
 printf(" Enter the item which you want to search");
 scanf("%d", &item);
 location = binarysearch (arr, 0, 9, item);
 if (location != -1)
 {
  printf( "Item found at location %d", location);
 }
 else
 {
  printf("item not found");
 }
}
int Binary search (int a[], int beg, int end, int item)
{
 int mid;
 if (end >= beg)
 {
  mid = (beg + end)/2;
  if (a[mid] == item)
   {
   return mid+1;
   }
  else if (a[mid] < item)
  {
  return binarysearch (a, mid +1, end, item);
  }
  else
```
<!-- corrected: p.78: the gaps inside the scanf quotes are this writer's normal spacing, not reliably spaces in the format; transcribed as "%d" (see P5-12) -->
(Definition name is written "Binary search" with capital B and a gap; prototype/calls are written "binarysearch".)

### PDF p.79 (notebook p.78)

```c
  {
  return binary search (a, beg, mid-1, item);
  }
 }
 return -1;
}
```
Output:
```text
Enter item which you want to search
19
Item found at location 2.
```

**Sorting Algorithm :-** (highlighted)
**1) Bubble Sort Algorithm :-**
Bubble sort algorithm is a simplest sorting algorithm. Bubble sort works on repeatedly swapping of adjacent elements until they are not in intended order. It is called as Bubble sort because moment of array elements is just like movement of air bubbles in the water. Bubbles in water rise up to the surface; similarly the array elements in bubble sort move to end in each iteration.
It is not suitable for large data sets. The average and worst case complexity of bubble sort is O(n²), where n is number of items.
Bubble sort is majorly used where :
- complexity does not matter
- simple and shortcode is preferred.

### PDF p.80 (notebook p.79)

**Algorithm :-**
```text
begin BubbleSort(arr)
  for all array elements
    if arr[i] > arr[i+1]
      swap(arr[i], arr[i+1])
    end if
  end for
  return arr
end Bubble sort.
```

**Bubble sort complexity :-**

| case | Time complexity | Space complexity |
|---|---|---|
| Best case | O(n) | O(1). |
| Average case | O(n²) | |
| Worst case | O(n²) | |

**Implementation of Bubble Sort :-**
C language Implementation :-
```c
#include <stdio.h>
void print(int a[], int n)
 {
  int i;
  for (i=0; i<n; i++)
   {
   printf("%d", a[i]);
   }
```

### PDF p.81 (notebook p.80)

```c
 }
void bubble (int a[], int n)
 {
 int i, j, temp;
 for (i=0; i<n; i++)
  {
   for (j=i+1 ; j<n ; j++)
   {
    if (a[j] < a[i])
     {
     temp = a[i];
     a[i] = a[j];
     a[j] = temp;
     }
   }
  }
}
void main ()
 {
  int i, j, temp;
  int a[5] = { 10, 35, 32, 13, 26 };
  int n = sizeof(a) / sizeof(a0);
  printf("Before sorting array elements are :\n");
  print(a, n);
  bubble(a, n);
  printf("\n after sorting array elements-\n");
  print(a, n);
}
```
(The `j=i+1` is written over an overwritten character; `sizeof(a0)` is written without brackets.)

### PDF p.82 (notebook p.81)

output :-
```text
Before sorting array elements are -
10 35 32 13 26
After sorting array elements are -
10 13 26 32 35.
```

**Bucket Sort Algorithm :-** (highlighted)
The data items in the bucket sort are distributed in form of buckets.
Bucket sort is a sorting algorithm that seprates elements into multiple groups said to be buckets. Elements in bucket sort are first uniformly divided into groups called buckets, and then they are sorted by any other sorting algorithm. After that, elements are gathered in sorted manner.

Advantages of bucket sort are :-
- Bucket sort reduces no. of comparisons
- It is asymptotically fast because of uniform distribution of elements.

limitations of bucket sort are :
- It may or may not be a stable sorting algorithm
- It is not useful if we have a large array bcz it increases the cost.
- It is not an in-place sorting algorithm, because some extra space is required to sort the buckets.

### PDF p.83 (notebook p.82)

The best and average-case complexity of bucket sort is O(n+k), worst-case complexity of bucket sort is O(n²), where n is number of items.

Bucket sort is commonly used :
- with floating-point values.
- when input is distributed uniformly over a range.

**Algorithm :-**
```text
Bucket sort (A[])
1. let B[0 .... n-1] be a new array.
2. n = length[A].
3. for i=0 to n-1
4. make B[i] an empty list
5. for i=1 to n
6. do insert A[i] into list B[n a[i]]
7. for i=0 to n-1
8. do sort list B[i] with insertion sort.
9. concatenate lists B[0], B[1] .... B[n-1] together in order.
10. END.
```

**Complexity :-** (highlighted)
1. Time complexity :-

| case | time complexity |
|---|---|
| Best case | O(n+k). |

### PDF p.84 (notebook p.83)

| case | time complexity |
|---|---|
| Average case | O(n+k) |
| worst case | O(n²) |

2. space complexity :-

| | |
|---|---|
| Space complexity | O(n*k) |
| stable | YES |

**Implementation of bucket sort in C :-** (highlighted)
```c
#include <stdio.h>
int getmax (int a[], int n)
{
 int max = a[0];
 for ( int i=1; i<n; i++)
  if (a[i] > max)
     max = a[i];
 return max;
}
void bucket (int a[], int n)
 {
 int max = getmax (a, n)
 int bucket[max], i;
 for ( int i = 0 ; i<=max ; i++)
  {
```
<!-- corrected: p.84: the word is max written with the writer's looped x (same shape as in int max above), not masu (see P5-27) -->
(In getmax, the assignment target is `max`, written with the writer's looped "x". The line `int max = getmax (a, n)` has no visible semicolon.) <!-- corrected: note rewritten as a plain reading of max -->

### PDF p.85 (notebook p.84)

```c
     bucket [i] = 0 ;
  }
 for(int i =0 ; i<n ; i++)
  {
   bucket [a[i]]++ ;
  }
 for (int i = 0 ; j=0 ; i<=max ; i++)
  {
  while (bucket [i] >0)
   {
    a[j++] = i ;
    bucket [i]-- ;
   }
  }
}
void printArr (int a[], int n)
 {
  for (int i=0; i<n ; i++)
  printf("%d", a[i]);
 }
int main ()
{
 int a[] = {54, 12, 84, 57, 69, 41, 9, 5};
 int n = sizeof(a) / sizeof a[0]);
 printf ("Before sorting array elements are:-\n");
 printArr(a, n);
 bucket (a, n);
 printf( "\n After sorting array elements are:-\n");
 printArr (a, n);
}
```
(`sizeof a[0])` — the opening parenthesis before `a[0]` appears as `of a[0])` i.e. "sizeof a[0])"; it may be "sizeof(a[0])" with a faint paren.)

### PDF p.86 (notebook p.85)

output :-
```text
Before sorting array elements are :-
54 12 84 57 69 41 9 5
After sorting array elements are :-
5 9 12 41 54 57 69 84.
```

**Heap Sort Algorithm :-** (highlighted)
Heap sort processes the elements by creating min-heap or max-heap using the elements of the given array.
Heap sort basically recursively performs two main operations :
- Build a heap H, using the element of array.
- Repeatedly delete the root element of heap formed in 1st phase.

What is heap ?
A heap is a complete binary tree, and binary tree is a tree in which node can have utmost two children.

**Algorithm :-**
```text
Heap Sort (arr)
BuildMaxHeap(arr)
for i = length (arr) to 2
  swap arr [1] with arr [i]
   heap-size [arr] = heap-size [arr] ? 1
   MaxHeapify (arr, 1)
End.
```
<!-- corrected: verified: the first index really is an undotted 1 and the second a dotted i -->

### PDF p.87 (notebook p.86)

```text
BuildMaxHeap (arr):
  BuildMaxHeap (arr)
    heap-size (arr) = length (arr)
    for i = length (arr)/2 to 1.
  MaxHeapify (arr, i)
  End.
```

**complexity :-**

| case | Time complexity | space complexity |
|---|---|---|
| Best | O(n log n) | O(1). |
| Average | O(n log n) | |
| worst | O(n log n) | |

**Implementation of Heap sort :-** (highlighted)
```c
#include <stdio.h>
/* function to heapify a subtree. Here is 'i' the
index of root node in array a[], and 'n' is size
of heap */
void heapify (int a[], int n, int i)
 {
  int largest = i
  int left = 2 * i+1
  int right = 2 * i+2
  if(left <n && a[left] > a[largest]
     largest = left ;
```

### PDF p.88 (notebook p.87)

```c
 if (right <n && a[right] > a[largest]
    largest = right ;
 if (largest != 1)
 { int temp = a[i] ;
   a[i] = a[largest];
   a[largest] = temp;
   heapify (a, n, largest);
 }
}
void heapsort (int a[], int n)
{
 for(int i = n/2-1 ; i>=0, i--)
   heapify (a, n, i);
 for (int i= n-1; i>0 , i--)
 { int temp = a[0];
   a[0] = a[i];
   a[i] = temp;
  heapify (a, i, 0);
 }
} void printArr (int arr[], int n).
{
  for(int i= 0; i<n ;++i)
   {
    printf( "%d", arr[i]);
    printf( " ");
   }
} int main ()
 {
 int a[] = { 48, 10, 23, 43, 28, 26, 1};
 int n = sizeof (a) / sizeof (a[0]);
 printf ("Before sorting array elements are - \n");
```
<!-- corrected: p.88: printArr has no space inside the quotes, then a separate printf(" ") (see P5-43) -->

### PDF p.89 (notebook p.88)

```c
 printArr (a, n);
 heapSort (a, n);
 printf("\n After sorting array elements are - \n");
 printArr (a, n);
 return 0;
}
```
output:
```text
Before sorting array elements are :-
48 10 23 43 28 26 1
After sorting array elements are -
1 10 23 26 28 43 48.
```

**Insertion sort Algorithm :-** (highlighted)
Insertion sort works similar to the sorting of playing cards in hands. It is assumed that the first card. The idea behind the insertion sort is that first make/take one element, iterate it through sorted array. complexity of insertion sort in the average case and worst case is O(n²), where n is number of items.
Insertion sort is less efficient than the other sorting algorithms like heap sort, quick sort and merge sort etc.

Insertion sort has various advantages such as :
- simple implementation.
- Efficient for small data sets.
- Adaptive i.e it is appropriate for data sets that are already substantially sorted.

### PDF p.90 (notebook p.89)

complexity :-
case time complexity :
- Best case O(n)          space complexity O(1).
- Average case O(n²)
- worst case O(n²).

**Implementation of insertion sort :-**
```c
#include <stdio.h>
void insert (int a[], int n)
{
  int i, j, temp ;
  for (i= 1; i<n , i++) {
   temp = a[i];
   j = i-1 ;
  while (j>=0 && temp <= a[j])
  {
   a[j+1] = a[j];
   j = j-1;
  }
  a[j+1] = temp;
 }
}
void printArr (int a[], int n)
 {
  int i;
  for(i=0; i<n; i++)
  printf ("%d", a[i]);
 }
int main()
{
 int a[] = { 12, 31, 25, 8, 32, 17};
```
(continues on PDF p.91)
