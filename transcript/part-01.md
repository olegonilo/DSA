# DSA Handwritten Notes (Topper World): PDF pages 1–18

> Verbatim transcription of the handwritten DSA notebook (English, as written), PDF pages 1–18.
> Original errors are intentionally preserved; only transcription mistakes were fixed (each marked with `<!-- corrected: ... -->`).
> For the list of errors in the notes themselves see [../errata/ERRATA-full.md](../errata/ERRATA-full.md).


### PDF p.1 (notebook p.?)
Printed cover page. Topper World logo top-right. Graphic: a blue circle holding a small tree/hierarchy icon (one yellow root node joined by white lines to three child nodes: light-blue, orange, blue).
Title: **DSA** (red) / **HANDWRITTEN NOTES**.
"Prepared By: [TopperWorld logo — 'TOPPERWORLD, LEARN & GROW']"

### PDF p.2 (notebook p.2)
**INDEX**

| Sr.No. | Title of Topic | Page No. |
|---|---|---|
| 1). | Data structure introduction. | 3 |
| 2). | classification of data structure | 7 |
| 3). | introduction to algorithm | 9 |
| 4). | asymptotic analysis | 15 |
| 5). | DS- pointer | 18 |
| 6). | DS- structure | 20 |
| 7). | DS- Array | 23 |
| 8). | DS- linked list | 30 |
| 9). | DS-skip list | 36. |
| 10). | DS-stack | 41 |
| 11). | DS-Queue | 44 |
| 12). | DS-Tree | 48. |
| 13). | Types of Tree | 58 |
| 14). | DS- Graph | 62 |

### PDF p.3 (notebook p.?)
(Index continued)

| Sr.No | Topic Name | Page No. |
|---|---|---|
| 15). | Graph Traversal Algorithms | 65 |
| 16). | searching | 71 |
| 17). | searching Algorithms with example | 82 |
| 18). | sorting Algorithms | 86. |
| 19). | Implementations of sorting Algorithms | 89. |
| 20). | Data structure interview questions with Answers (50 questions with Answers) | 95. |
| 21). | DATA STRUCURE CODING QUE. | 104. |
| 22). | END | |

### PDF p.4 (notebook p.3)
**What is Data Structure ?** (highlighted)

→ Data structure is a way to store and organize data so that it can be used efficiently.
As per name indicates itself that organizing the data in memory.
The data structure is not any programming language like c, c++, Java etc. It is set of algorithms that we can use in any programming language to structure data in memory.

Diagram (tree with red arrows):
```
                       Data structures
                 ┌───────────┴────────────┐
   primitive data structure      Non-Primitive Datastructure
   ┌─────┬──────┬──────┬───────┐          ┌──────────┴──────────┐
  int  pointer char  float  double    linear D.S.          Non linear D.S.
```
(The "pointer" arrow drops from the int/char branch line down beneath "int"/"char".)

**Linear Data structure :-**
The arrangement of data in the sequential manner is known as linear data structure. The data structure used for this purpose are **Arrays, linked list, stacks and queues.**
In this data structures, one element is connected to only one another element in a

### PDF p.5 (notebook p.4)
linear form.

**Non-linear data structure :-**
When one element is connected to the 'n' number of elements known as non-linear data structures.
Example :- **trees and graphs.**
In this case, elements are arranged in a random manner.

**Algorithms and Abstract Data types ??** (highlighted)

Diagram: three rounded boxes stacked vertically, joined by downward red arrows:
`[Algorithms] → [Abstract data types] → [set of rules]`

Why →
To structure the data in memory, 'n' number of algorithms are proposed, and all these algorithms are knowns as **Abstract Data Types.**

### PDF p.6 (notebook p.5)
An Abstract Data Type tells **what is to be done** and data structure tells **how is to be done** ?

ADT gives us the blueprint while data structure provides the implementation part.

**What is Data ?**
Data can be defined as the elementary value / collection of values.
for example :- student's name and its id are the data about student.

**What is Record ?**
Record can be defined as collection of various data items
example :- student entity → name, address, course and marks can be grouped together to form record.

**What is File ?**
File is a collection of various records of one type of entity
example :- if there are 60 employees in class, then there will be 20 records in related file where record contains info of employee

**What is Attribute and Entity ?**
An entity represents class of certain objects. it contains various attributes. each attribute represents particular property of that entity.

### PDF p.7 (notebook p.6)
**What is need of data structures ?** (highlighted)
As applications are getting complexed and amount of data is increasing day by day, there may arrise following problems :-
**Processor speed :-** As data is growing day by day to the billions of files per entity, processor may fail to deal with that amount of data.
**Data Structure :-** consider an inventory size of 106 items in store, if our application needs to search for a particular item, it needs to transverse 106 items every time, results in slowing down process.
**multiple requests :-** If thousands of users are searching data simultaneously on a web server, then there are chances that to be failed to search during that process.
To solve this problems, data structures are used. Data is organized to form a data structure in a such way that all items are not required to be searched and require data can be searched instantly.

**Advantages of data Structure :-**
**Efficiency :-** If the choice of a data structure for implementing a particular ADT is proper, it makes program very efficient in terms of time and space.
**Reusability :-** The data structure provides reusability means that multiple client programs can use the data structure.

### PDF p.8 (notebook p.7)
**Abstraction :-** The data structure specified by the ADT also provides level of abstraction. The client cannot see internal working of data structure, so it does not have to worry about implementation.

\* **Data structure classification :-** (highlighted)

Diagram (red arrows):
```
                           Data structure
              ┌──────────────────┴───────────────────┐
   primitive data structure              Non-Primitive Data structure
                                        ┌────────────┴─────────────┐
                                      Linear                   Non-linear
                                ┌───────┴───────┐             ┌────┴────┐
                              Static         Dynamic          Tree     Graph
                                │       ┌───────┼────────┐
                              Array  Linked list  Stack  Queue
```
(Array, Linked list, Stack, Queue written in red.)

### PDF p.9 (notebook p.8)
**Operations on data structure :-** (highlighted)
1). **Traversing :-** Every data structure contains a set of data elements. Traversing data structure means visiting each element of data structure in order to perform some specific operation like searching or sorting.
Example :- If we need to calculate average of marks obtained by a student in 6 different subject, we need to traverse complete array of marks and calculate total sum, then we will devide that sum by no. of subjects i.e. 6 to find average.

2). **Insertion :-** Insertion can be defined as the process of adding the elements to the data structure at any location.
If the size of data structure is n then we can only insert n-1 data elements to it.

3). **Deletion :-** The process of removing an element from the data structure is called deletion. we can delete an element from data structure at any random location.
If we try to delete an element from an empty data structure then underflow occurs.

4). **Searching :-** The process of finding the location of an element within data structure is called searching. There are two algorithms to perform

### PDF p.10 (notebook p.9)
searching, linear search and Binary search.

5). **Sorting :-** The process of arranging the data structure in a specific order is called as sorting. There are many algorithms that can be used to perform sorting, for example, insertion sort, selection sort, bubble sort etc.

6). **merging :-** When two lists list A and list B of size M and N respectively, of similar type of elements, clubbed or joined to produce third list, list C of size (M+N), then this process is called merging.

**DATA STRUCTURES AND ALGORITHM** (red, large)

**What is Algorithm ?** (highlighted)
An algorithm is a process or a set of rules required to perform calculations or some other problem-solving operations especially by a computer.
It is not complete program or code ; it is just a solution (logic) of a problem, which can be represented either as an informal description using a flowchart or pseudocode.

**characteristics of an algorithm.**
**Input :-** An algorithm has some input values. We can pass 0 or some input value to an algorithm.

### PDF p.11 (notebook p.10)
**Output :-** We will get 1 / more output at end of an algorithm.

**unambiguity :-** An algorithm should be unambigous which means that instruction in an algorithm should be clear and simple.

**finiteness :-** An algorithm should have finiteness. means limited number of instructions.

**Effectiveness :-** An algorithm should have finite as each instruction in an algorithm affects the overall process.

**Approches in Algorithm :-** (highlighted)
1). **Brute force Algorithm :-** The general logic structure is applied to design an algorithm. It is also known as exhaustive search algorithm that searches all possible to provide required solution.

such algorithms have two types :-

| 1). optimizing | 2). sacrificing |
|---|---|
| finding all solutions of a problem and then take out the best solution is known then it will terminate if the best solution is known. | As soon as the best solution is found, then it will stop. |

### PDF p.12 (notebook p.11)
**Divide and conquer :-** This breaks down the algorithm to solve the problem in different methods. It allows you to break down problem into different methods, and valid output is produced for the valid input. This valid output is passed to some other function.

**Greedy algorithm :-** It is an algorithm paradigm that makes an optimal choice on each iteration with the hope of getting best solution. It is easy to implement and has faster execution time. But there are very rare cases in which it provides the optimal solution.

The **major categories of algorithms** are given below:
**Sort :-** Algorithm developed for sorting the items in a certain order.
**search :-** Algorithm developed for searching the items inside a data structure.
**Delete :-** Algorithm developed for deleting the existing element from the data structure.
**Insert :-** Algorithm developed for inserting an item inside a data structure.
**Update :-** Algorithm developed for updating the existing element inside a data structure.

### PDF p.13 (notebook p.12)
**Algorithm Analysis :-** (highlighted)
The algorithm can be analyzed in two levels i.e. first is before creating the algorithm, and second is after creating the algorithm.
There are two analysis of an algorithm.
**Priori Analysis :-** Here, priori analysis is the theoretical analysis of an algorithm which is done before implementing the algorithm.

**Posterior Analysis :-** Here, posterior analysis is a practical analysis of an algorithm. The practical analysis is achieved by implementing algorithm using any programming language.

**Algorithm complexity :-** The performance of the algorithm can be measured in two factors :
**Time complexity :-** The time complexity of an algorithm is the amount of time required to complete the execution. The time complexity of an algorithm is denoted by the big O notation.
Here big O notation is the asymptotic notation to represent time complexity.
The time complexity is mainly calculated by counting the number of steps to finish execution.

### PDF p.14 (notebook p.13)
```text
sum = 0 ;
// suppose we have to calculate the sum of n
   numbers.
for i=1 to n
sum = sum + i ;
// when the loop ends then sum holds the sum
   of n numbers.
return sum;
```
In above code, the time complexity of the loop statement will be atleast n, and if value of n increases, then time complexity also increases.
We generally consider the worst-time complexity as it is maximum time taken for any given input size.

**Space complexity :-** An algorithm's space complexity is the amount of space required to solve a problem and produce an output. Similar to the time complexity, space complexity is also expressed in big O notation.

**Space complexity = Auxiliary space + Input size.**

### PDF p.15 (notebook p.14)
The following are the types of algorithms :
**Search Algorithm :-** on each day, we search for something in our day to day life.
Similarly, with the case of computer, huge data is stored in a computer that whenever user asks for any data then the computer searches for that data in the memory and provides that data to the user. There are mainly two techniques available to search data in an array :
- Linear search
- Binary search

**sorting Algorithms :-** sorting algorithms are used to rearrange elements in an array or a given data structure either in an ascending or descending order. The comparison operator decides the new order of the elements :

(Rest of page blank.)

### PDF p.16 (notebook p.15)
**Asymptotic Analysis :-** (highlighted)
The time required by an algorithm comes under three types :
**Worst case :-** It defines the input for which the algorithm takes a huge time.

**Average case :-** It takes average time for the program execution.

**Best case :-** It defines the input for which the algorithm takes the lowest time.

**Asymptotic Notations :-** The commonly used asymptotic notations used for calculating the running time complexity of an algorithm is given below :
1). **Big oh notation (O) :-** This measures the performance of an algorithm by simply providing the order of growth of the function.
This notation provides an upper bound on a function which ensures that function never grows faster than the upper bound.

Diagram: x/y axes with an upward arrow on the y-axis and a rightward arrow on the x-axis. A dashed straight line labelled **g(n)** and a solid wavy curve labelled **f(n)** both start at the origin. Near the origin f(n) bulges slightly above g(n); they cross at a point marked by a small vertical tick labelled **k** on the x-axis, and after k the dashed g(n) stays above f(n). (The upper curve is labelled g(n), not c·g(n).)

### PDF p.17 (notebook p.16)
Example :- If f(n) and g(n) are two functions defined for positive integer,
then **f(n) = O g(n)** as f(n) is big oh of g(n) or f(n) is on order of g(n)) if there exists constants c and n0 such that :
**f(n) ≤ c · g(n) for all n ≥ n0**

2). **Omega Notation (Ω) :-** It basically describes best case scenario which is opposite to big o notation. It is the formal way to represent lower bound of an algorithm's running time.

Diagram: axes (y up, x → labelled n). Three curves start near the origin: top **c2 g(n)**, middle **f(n)**, bottom **c1 g(n)**. To the left of a dashed vertical line at **n0** they cross and tangle (f(n) dips below c1 g(n) near n0); to the right, f(n) lies between c1 g(n) and c2 g(n). Caption under the x-axis: **n0  f(n) = Θ(g(n))**.

Example :- let f(n) and g(n) be functions of n where n is steps required to execute program.
**f(n) = Θ g(n)**
The above condition is satisfied only if when :
**c1.g(n) <= f(n) <= c2.g(n)**

### PDF p.18 (notebook p.17)
2). **omega Notation (Ω)**
It basically describes best-case scenario which is opposite to big-o notation. It is formal way to represent lower bound to an algorithm's running time. It measures the best amount of time an algorithm can possibly take to complete or best case time complexity.
Example :- If f(n) and g(n) are two functions defined for positive integers,
then **f(n) = Ω g(n)** as f(n) is omega of g(n) or f(n) is on the order of g(n) if there exists constants c and n0 such that :
**f(n) >= c.g(n) for all n ≥ n0 and c>0**

Diagram: "y axis" vertical arrow, "x axis" horizontal arrow. Two rays from near the origin: **f(n)** (steeper, wavy near the origin — it rises, falls back below the other line, then crosses it) and **c.g(n)** (shallower straight line). They cross at a point; a downward arrow from the crossing marks **n0** on the x-axis. To the right of n0, f(n) is above c.g(n).

3). **Theta Notation (Θ)**
The theta notation mainly describes average case scenarios.
It represents realistic time complexity of an algorithm. Big theta is mainly used when the value of worst-case and best-case is same.
