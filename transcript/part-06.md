# Part 06 — PDF pages 91–109

> Verbatim transcription of the handwritten DSA notebook (English, as written), PDF pages 91–109.
> Original errors are intentionally preserved; only transcription mistakes were fixed (each marked with `<!-- corrected: ... -->`).
> For the list of errors in the notes themselves see [../errata/ERRATA-full.md](../errata/ERRATA-full.md).

> **Page order:** in this part the PDF order differs from the notebook order. PDF p.91–93 = notebook p.90–92; PDF p.94–98 = notebook p.104–108 (coding questions); PDF p.99–109 = notebook p.93–103 (interview questions Q1–Q50). The sections below follow PDF order. The merge-sort section stops after `merge()` and the complexity table; `mergeSort()` and `main()` are never written.


### PDF p.91 (notebook p.90)

(End of insertion sort `main()` from the previous page.)

```c
int n = sizeof (a) / sizeof a[0]);
printf("Before sorting array elements are -\n");
printArr(a, n);
insert(a, n);
printf("\nAfter sorting array elements are -\n");
printArr(a, n);
return 0;
}
```
<!-- corrected: p.91: the page has unbalanced parentheses, sizeof a[0]) (see P6-N1) -->

Output:
```
Before sorting array elements are -
12 31 25 8 32 17
After sorting array elements are -
8 12 17 25 31 32.
```

**Merge Sort Algorithm :-** (highlighted)
Merge sort is the sorting technique that follows divide and conquer approach. This will be very helpful and interesting.
Merge sort is similar to the quick sort algorithm as it uses the divide and conquer approach to sort elements.

**Algorithm :-**
arr is given array, beg is starting element and end is last element of array.
```
MERGE-SORT (arr, beg, end)
if beg < end
Set mid = (beg + end)/2.
MERGE-SORT (arr, beg, mid)
MERGE-SORT (arr, mid+1, end)
MERGE (arr, beg, mid, end)
```

### PDF p.92 (notebook p.91)

```
end of if
End MERGE_SORT.
```

implementation of merge sort:-
```c
/* function of merge the subarrays of a[] */
void merge (int a[], int beg, int mid, int end)
{
 int i, j, k;
 int n1 = mid - beg + 1;
 int n2 = end - mid;
 int LeftArray[n1], RightArray[n2];
 /* copy data to temp arrays */
 for (int i = 0; i < n1; i++)
 LeftArray[i] = a[beg + i];
 for (int j = 0; j < n2; j++)
 RightArray[j] = a[mid + 1 + j];
 i = 0;
 j = 0;
 k = beg;
while (i < n1 && j < n2)
{
 if (LeftArray[i] <= RightArray[j])
 {
  a[k] = LeftArray[i];
  i++;
 }
 else
 {
 a[k] = RightArray[j];
 j++;
 }
```

### PDF p.93 (notebook p.92)

```c
 }
 k++;
 }
 while (i < n1)
 {
 a[k] = LeftArray[i];
  i++;
  k++;
 }
 while (j < n2)
 {
  a[k] = RightArray[j];
  j++;
  k++;
 }
}
```

Complexity :-

| case | Time complexity | space complexity |
|---|---|---|
| Best case | o(n*logn) | o(n). |
| Avg. case | o(n*logn) | |
| worst case | o(n*logn) | |

(The rest of the page is blank. No `mergeSort` C function or `main` follows.)

### PDF p.94 (notebook p.104)

**DATA STRUCTURE CODING QUE.** (title, "CODING QUE." highlighted)

Arrays by using c :
1). program to demonstrate arrays in c
```c
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
# define NUM_EMPLOYEE 10
int main (int argc, char *argv[]}
{ int salary [NUM_EMPLOYEE], lcount=0,
   gcount=0 ,i=0;
   printf ("Enter employee salary (MAX 10)\n");
    for (i=0; i < NUM_EMPLOYEE ; i++)
   {
     printf("\n Enter employee salary : %d -",
              i+1);
     scanf ("%d", &salary [i]);
   }
    for(i=0; i<NUM_EMPLOYEE ; i++)
   {  if (salary [i] < 3000]
      lcount ++;
     else
      gcount ++;
   }
   printf ("\n There are {%d} employee with
          salary more than 3000 \n", gcount);
   printf("There are {%d} employee with salary
          less than 3000 \n", lcount);
   printf("press Enter to continue ...\n");
```
<!-- corrected: p.94: the condition closes with ] not ) (see P6-N2) -->
<!-- corrected: p.94: in this second for loop NUM-EMPLOYEE looks hyphenated (mid-height stroke), while the #define uses a baseline underscore; ambiguous -->
(`char *argv[]}` is written with a closing brace `}` instead of `)`. The printf string literals are broken across lines in the handwriting.)

### PDF p.95 (notebook p.105)

```c
   getchar ();
   return 0;
}
```

2). Linked list in c++ :
```cpp
using namespace std;
template <typename T>
class node
{
  public :
  T value;
  Node *next;
  Node *previous;
  Node (T value)
  {
    this->value = Value;
  }
};
 template <typename T >
 class Linked list
 {
  private :
  int size ;
  Node <T> *head_ = NULL;
  Node <T> *tail_= NULL;
  Node <T> *itr = NULL;
  public ;
  Linked list ()
  {
   this->size_ = 0;
  }
```
<!-- corrected: p.95: the right-hand V is noticeably larger and probably a capital (ambiguous reading; see P6-N4) -->
<!-- corrected: p.95: written with a gap, head - = NULL (read as head_) -->
(Written "class node" in lowercase, but members use "Node". The class name is written "Linked list" with a space. `public ;` is written with a semicolon. The member is declared `size` but used as `size_`.)

### PDF p.96 (notebook p.106)

```cpp
   void append (T value)
   {
    if (this->head == NULL)
   {
    this->head_ = new Node <T> (value);
    this->tail = this->head_;
    }
   else
    {
    this->tail_->next = new node <T> (value);
    this->tail_->next->previous = this->tail_
                                 ->tail;
    }
    this->size_ += 1;
   }
  void prepend (T value)
  {
  void resetIterator ()
  {
  tail_ = NULL;
  }
int main (int argc, char **argv)
{
  LinkedList <int> lList ;
  llist. append (10);
  llist.append (3);
  llist .append (1);
 cout << "printing linked list << endl;
 cout << endl ;
 return 0;
}
```
<!-- corrected: p.96: written with a gap, this->size_ + = 1; -->
<!-- corrected: p.96: the three calls use llist (lowercase l), the declaration lList (see P6-N3) -->
<!-- corrected: p.96: the endl at the end of this cout line is written in a stylised way, like end-l -->
(`prepend` has an opening brace and no body or closing brace. `this->tail_->next->previous = this->tail_` continues on the next line as `->tail;`, so it reads either `this->tail_` or `this->tail_->tail`. The class is never closed with `};`. The string `"printing linked list << endl;` has no closing quote.)

### PDF p.97 (notebook p.107)

3). stack implementation in c :
```c
#include <stdio.h>
int MAXSIZE = 8;
int stack [8];
int top = -1;
int isempty () {
  if (top == -1)
    return 1;
  else
    return 0;
 }
int isfull () {
  if (top == MAXSIZE)
    return 1;
  else
    return 0; }
int peek () {
  return stack [top]; }
int pop () {
  int data;
  if (!isempty ()) {
   data = stack [top];
   top = top-1;
   return data; }
  else {
   printf ("could not retrieve data, stack is empty\n");
  }
}
int push (int data) {
 if (!isfull ()) {
 top = top+1;
```

### PDF p.98 (notebook p.108)

```c
  stack [top] = data;
  } else {
   printf("could not insert data, stack is full\n");
  }
}
int main () {
 // push items on to the stack
 push (3);
 push (5);
 push (9);
 push (1);
 push (12);
 push (15);
 printf("Element at top of the stack : %d\n", peek());
 printf(" Elements : \n");
// print stack data
 while (!isempty ()) {
   int data = pop ();
   printf ("%d \n", data);
 }
printf("stack full : %s\n", isfull () ? "true" : "false");
printf("stack empty : %s\n", isempty() "true", "false");
return 0;
}
```
(The third push reads "push (g)" in the handwriting, meaning 9. The last printf has no `?` and uses `,` where `:` belongs.)

### PDF p.99 (notebook p.93)

**DATA STRUCTURES INTERVIEW QUESTIONS WITH ANSWERS** (highlighted)

**Q.1. What is data structure?**
→ A data structure is a way of organizing data that considers not only items stored, but also their relationship to each other.

**Q.2. List out the areas in which data structure are applied extensively?**
→ • compiler design, • operating system, • database system, • statistical analysis, • numerical analysis, • artificial intelligence

**Q.3. What are major data structures used in following areas: Rdbms, network data model and Hierarchical data model.**
→ Rdbms = array (array of structures).
network data model = graph.
Hierarchical data model = tree.

**Q.4. If you are using c language to implement the heterogeneous linked list, what pointer type will you use?**
→ The heterogeneous linked list contains different data types in its nodes and we need a link, pointer to connect them. It is not possible to use ordinary pointer for this. so we go for void pointer. void pointer is capable of storing pointer to any

### PDF p.100 (notebook p.94)

type as it is a generic pointer type.

**Q.5. minimum number of queues needed to implement the priority queue?**
→ two. one queue is used for actual storing of data and another for storing priorities.

**Q.6. What is data structure used to perform recursion?**
→ stack. because of its LIFO (Last IN First Out) property it remembers its 'caller'.

**Q.7. What are notations used in evaluation of arithmetic expressions using prefix & postfix forms?**
→ Polish and Reverse polish notations.

**Q.8. convert expression ((a+b)*c-(d-e)^(f+g)) to equivalent prefix and Postfix notations.**
→ prefix notation : - * + a b c ^ - d e + f g
postfix notation : a b + c * d e - f g + ^ -

**Q.9. What are methods available in storing sequential files?**
→ 1. straight merging,
2. natural merging,
3. polyphase sort,
4. distribution of initial runs.

**Q.10. Whether linked list is a linear or non-linear data structure?**

### PDF p.101 (notebook p.95)

→ According to access strategies linked list is a linear one. according to storage linked list is a non linear one.

**Q.11. define doubly linked list.**
→ It is collection of data elements called nodes, where each node is divided into three parts :
• an info field that contains information stored in the node.
• left field that contain pointer to node on left side.
• Right field that contain pointer to node on right side.
•

**Q.12. What are the Issues that hampers efficiency in sorting a file?**
→ • length of time required by programmer in coding a particular sorting program.
• amount of machine time necessary for running the particular program.
• amount of space necessary for particular pgm.
• object oriented analysis and design.

**Q.13. calculate efficiency of sequential search?**
→ The number of comparisons depends on where the record with argument key appears in table
• If it appears at first position then one comparison.
• If it appears at last position then n comparison.
• average = (n+1)/2 comparisons.

### PDF p.102 (notebook p.96)

• number of comparisons in any case is O(n).

**Q.14. Is any implicit arguments are passed to a function when it is called?**
→ yes, there is a set of implicit arguments that contain information necessary for function to execute and return correctly, one of them is return address which is stored within the function's data area, at time of returning to calling program address is retrived and function branches to that location.

**Q.15. Paranthesis is never required in postfix or prefix expressions? Why**
→ parenthesis is not required because order of the operators in postfix / prefix expressions determines actual order of operations in evaluating expression.

**Q.16. List out few of applications of tree data structure?**
→ The manipulation of arithmatic expression, symbol table construction & syntax analysis.

**Q.17. List out few of applications that make use of multilinked structures?**
→ sparse matrix, Index generation.

**Q.18. What is type of the algorithm used in solving 8 queens problem?**

### PDF p.103 (notebook p.97)

→ backtracking.

**Q.19. In an AVL Tree, at what condition balancing is to be done?**
→ If 'pivotal value' or height factor is greater than 1 or less than -1.

**Q.20. In Rdbms, what is the efficient data structure in internal storage representation.**
→ b+ tree. because b+ tree, all the data is stored in only in leaf nodes, that makes searching easier. this corresponds to records that shall be stored in leaf nodes.

**Q.21. What is difference between array and a stack?**
→ Stack follows LIFO. thus the item that is first entered would be last to be removed.
In the array, items can be entered or removed by in any order. basically, each member access is done using index. no strict order is to be followed here to remove a particular element.

**Q.22. How to check whether a linked list is circular?**
→ create two pointers, each set to start of list. update each as follows :
```c
while (pointer1)
{
 pointer1 = pointer1->next;
 pointer2 = pointer2->next;
 if (pointer2) pointer2 = pointer2->next;
```

### PDF p.104 (notebook p.98)

```c
 if (pointer1 == pointer2)
 {
 print ("circularn");
 }
}
```
<!-- corrected: p.104: the string is written "circularn" (stray n), not "circular" -->

**Q.23. What is a node class?**
→ A node class is class that, relies on the base for service and implementation, provides a wider interface to users than its base class, relies primarily on virtual functions in its public interface depends on all its direct and indirect base class.

**Q.24. When can you tell that a memory leak will occur?**
→ a memory leak occurs when a program loses the ability to free a block of dynamically allocated memory.

**Q.25. What are types of collision Resolution techniques and methods used in each of the type?**
→ open addressing (closed hashing), methods used include: overflow block. closed addressing (open hashing) methods used include: linked list, binary tree

**Q.26. Which is simplest file structure? (sequential, index, random).**
→ Sequential is the simplest file structure.

### PDF p.105 (notebook p.99)

**Q.27. What are the notations, used in evaluation of arithmatic expression, using prefix and postfix forms?**
→ Polish and Reverse polish notations.

**Q.28. list out few of applications of tree data structure?**
→ The manipulation of arithmatic expressions, symbol table construction and syntax analysis.

**Q.29. difference between calloc and malloc?**
→ malloc : allocate n bytes.
calloc : allocate m times n bytes initialized to 0.

**Q.30. Which file contains the definition of member function**
→ defination of member function for the linked list class are contained in linkedlist.cpp file.

**Q.31. How is the front of the queue calculated?**
→ The front of the queue is calculated by
front = (front + 1) % size.

**Q.32. Why is the Isempty() member method called?**
→ the isempty() member method is called within the dequeue process to determine if there is an item in dequeue to be removed i.e. isempty() is called to decide whether queue has atleast one element. This method is called by dequeue() method before returning front element.

**Q.33. Which process places data at back of queue?**

### PDF p.106 (notebook p.100)

→ enque is a process that places data at back of the queue.

**Q.34. What is queue?**
→ A queue is sequential organization of data. a queue is a first in first out type of data structure. an element is inserted at last position and an element is always taken out from first position.

**Q.35. What does isempty() member method determine?**
→ isempty() checks if stack has at least one element. this method is called by pop() before retrieving and returning top element.

**Q.36. What method removes value from top of a stack?**
→ The pop() member method removes value from top of a stack, which is then returned by the pop() member method to statement that calls pop() member method.

**Q.37. What method is used to place a value onto the top of a stack?**
→ push() method, push is the direction that data is being added to stack. push() member method places a value onto the top of a stack.

**Q.38. How do you assign an address to an element**

### PDF p.107 (notebook p.101)

of a pointer array?
→ We can assign a memory address to an element of a pointer array by using the address operator, which is ampersand (&), in an assignment statement such as ptremployee[0] = &projects[2];

**Q.39. How many parts are there in a declaration statement?**
→ There are two main parts, variable identifier & data type and third type is optional which is type qualifier like signed/unsigned.

**Q.40. list some of the static data structures in c?**
→ Some of the static data structures in c are arrays, pointers, structures etc.

**Q.41. define dynamic data structure?**
→ A data structure formed when number of data items are not known in advance is known as dynamic data structure or variable size data structure.

**Q.42. list some of dynamic data structures in c?**
→ some of dynamic data structures in c are linked lists, stack, queues, trees etc.

**Q.43. define linear data structure.**
→ linear data structures are data structures having a linear relationship between its adjacent elements.
eg : linked list.

### PDF p.108 (notebook p.102)

**Q.44. define non-linear data structures.**
→ Non linear data structure are the data structures are data structure that don't have a linear relationship between its adjacent elements but have a hierarchical relationship between the elements.
eg : trees and graphs.

**Q.45. state the different types of linked lists?**
→ The different types of linked list include singly linked list, doubly linked list and circular linked list.

**Q.46. List the basic operations carried out in a linked list?**
→ • creation of a list
• Insertion of a list.
• deletion of a node.
• modification of a node.
• traversal of a node.

**Q.47. define a stack.**
→ Stack is an ordered collection of an elements in which insertion and deletions are restricted to one end. The end from which elements are added and or removed is referred as top of stack.

**Q.48. List out the basic operations that can be performed on a stack.**
→ • push operation.

### PDF p.109 (notebook p.103)

• pop operation
• peek operation
• empty check
• fully occupied check.

**Q.49. State the different ways of representing expression**
→ • Infix notation.
• prefix notation
• postfix notation.

**Q.50. What is sequential search?**
→ In sequential search each item in the array is compared with the item being searched until a match occurs.

(Rest of page blank.)
