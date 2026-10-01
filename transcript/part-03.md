# DSA Handwritten Notes (Topper World) — Part 03: PDF pages 37–54

> Verbatim transcription of the handwritten DSA notebook (English, as written), PDF pages 37–54.
> Original errors are intentionally preserved; only transcription mistakes were fixed (each marked with `<!-- corrected: ... -->`).
> For the list of errors in the notes themselves see [../errata/ERRATA-full.md](../errata/ERRATA-full.md).


### PDF p.37 (notebook p.36)

**3). Deletion and Traversing :-**

① **Deletion at begining** :- Removing the node from beginning of the list.
② **Deletion at end** :- Removing the node from end of the list.

**Traversing** :- visiting each node of the list atleast once in order to perform some specific operation like searching, sorting, display etc.

**Searching** :- comparing each node data with the item to be searched and return location of the item in the list if the item found else return null.

**Skip list :-** (highlighted)

\* **What is a skip list ?**
A skip list is a probalistic data structure. The skip list is used to store a linked list of elements or data with a linked list. In one single step, it skips several elements of the entire list, which is why it is known as skip list.

**Structure of skip list :-**
skip list is built in two layers: The lowest layer and the top layer. The lowest layer of the skip list is a common sorted linked list, and the top layers of the skip list are the like an "express line" where elements are skipped.

### PDF p.38 (notebook p.37)

**complexity table :-**

| Sr.No | complexity | Average case | Worst case |
|---|---|---|---|
| 1). | Access complexity | O(log n) | O(n) |
| 2). | search comple. | O(log n) | O(n) |
| 3). | delete comple. | O(log n) | O(n) |
| 4). | Insert comple. | O(log n) | O(n) |
| 5). | Space comple. | — | O(n log n) |

**Basic operations and its algorithms :-**
1). **Insertion operation** :- It is used to add new node to a particular location in a specific situation.
2). **Deletion operation** :- It is used to delete a node in a specific situation.
3). **search operation** :- The search operation is used to search a particular node in a skip list.

**Algorithm of insertion operation :-**
```text
Insertion (L, key)
local update [0 ... max-level +1]
a = L → header
for i = L → level down to 0 do.
        while a → forward [i] → key forward [i]
update [i] = a
```

### PDF p.39 (notebook p.38)

(continuation of insertion)
```text
a = a → forward[0]
lvl = random-level()
if lvφ > L → level then
for i = L → level + 1 to lvl do
    update [i] = L → header
    L → level = lvl
a = make node (lvl, key, value)
for i = 0 to level do
  a → forward [i] = update [i] → forward [i]
update [i] → forward [i] = a
```
(the condition is written "lvφ" — a slip for "lvl")

**Algorithm of deletion operation :-** (highlighted)
```text
Deletion (L, key)
local update [0.... max level +1]
a = L ↛ header
for i = L → level down o to do.
  while a → forward [i] → key forward [i]
    update [i] = a
a = a → forward [0]
if a → key = key then
  for i = 0 to L → level do
  if update [i] → forward [i] ? a then break
  update [i] → forward [i] → forward [i]
free(a)
while L → level > 0 and L → header → forward [L → level]
                                         = NIL do
  L → level = L → level -1.
```
(the "?" stands for a symbol that cannot be read, probably "≠"; "down o to" is written as shown)

### PDF p.40 (notebook p.39)

**Algorithm of searching operation :-**
```text
searching (L, Skey)
  a = L → header
  loop invariant : a → key level down to 0 do.
     while a → Forward [i] → key forward [i]
  a = a → forward [a]
  if a → key = skey then return a → value
  else return failure.
```

**Example :** create a skip list, we want to insert these following keys in empty skip list
1. 6 with level 1
2. 29 with level 1
3. 22 with level 4.
4. 9 with level 3.
5. 17 with level 1.
6. 4 with level 2.

→ **Solution** :- Insert 6 with level 1.
Diagram: a "Header" column of 5 stacked cells. Row labels from top: 3, 2, 1, 0, key (key is the bottom row). One arrow goes from the header to node **6**, which is drawn as two cells (a level-0 pointer cell above the key cell). The header arrow is drawn at the level-1 row, not at level 0. <!-- corrected: node 6 is drawn as two cells, not one; header arrow sits at the level-1 row (see P3-NEW-1) -->

**step 2 :- Insert 29 with level 1.**
Diagram: the same header column (3, 2, 1, 0, key). Arrows go header → node **6** → node **29**, drawn at the level-2 row (not at level 0). Each node is drawn as two cells: a level-0 pointer cell above the key cell. <!-- corrected: nodes are drawn as two cells; the arrows sit at the level-2 row (see P3-NEW-1) -->

### PDF p.41 (notebook p.40)

All diagrams use a Header column with rows 3, 2, 1, 0, key. Node towers stand on the key row.

**Step 3 : Insert 22 with level 4.**
- Header level 3 → 22, level 2 → 22, level 1 → 22. These are long red arrows.
- Level 0: header → 6 → 22 → 29.
- Tower sizes: 6 has 1 level cell, 22 has 4 level cells (0–3), 29 has 1.

**Step 4: Insert 9 with level 3.**
- Header level 3 → 22.
- Header level 2 → 9 → 22.
- Header level 1 → 9 → 22. The header-to-9 arrow is drawn broken.
- Level 0: header → 6 → 9 → 22 → 29.
- Tower 9 has 3 cells (levels 0–2).

**Step 5: Insert 17 with level 1** (header)
- Level 3: header → 22.
- Level 2: header → 9 → 22.
- Level 1: header → 9 → 22.
- Level 0: header → 6 → 9 → 17 → 22 → 29.
- Tower 17 has 1 cell.

**Step 6: Insert 4 with level 2.** (header)
- Level 3: header → 22.
- Level 2: header → 9 → 22.
- Level 1: header → 4 → 9 → 22.
- Level 0: header → 4 → 6 → 9 → 17 → 22 → 29.
- Tower 4 has 2 cells (levels 0–1).

### PDF p.42 (notebook p.41)

**Stack** :- A stack is a linear data structure that follows LIFO (Last-In-First-Out) principle. Stack has one end, whereas queue has two ends (front and rear).
A stack is a container in which insertion and deletion can be done from the end (one) known as the top of the stack.
A stack is an Abstract Data Type with a pre-defined capacity, which means that it can store elements of limited size.

**Operations on the stack :-** (highlighted)
1). **push ()** :- When we insert an element in a stack then the operation is known as push. If stack is full overflow condition occurs.
2). **pop ()** :- when we delete an element from stack, the operation is called as pop (). If stack is empty means no element exists in the stack, this state is known as an underflow state.
3). **peek ()** :- It returns the element at a given position.
4). **count ()** :- It returns the total number of elements available in a stack.
5). **change ()** :- It changes the element at the given position.
6). **display ()** :- It prints all the elements available in the stack.

**PUSH operation :-**
steps — Before inserting an element in the a stack, we check whethere the stack is full.

### PDF p.43 (notebook p.42)

- If we try to insert element in a stack, and the stack is full, then overflow condition occurs.
- when we initialised a stack, we set the value of top as -1 to check that stack is empty.
- The elements will be inserted until we reach the max size of the stack. top = top+1.

Diagram (fig: PUSH operation). Four stack boxes from left to right, joined by curved arrows labelled "push 10", "push 20" and "push 30":
1. Empty stack, label `top=-1`, caption "empty".
2. Holds [10], label `top=0`.
3. Holds [10, 20] from bottom up, label `top=1`.
4. Holds [10, 20, 30], label `top=2`, caption "Stack is full". An empty cell is still drawn above 30.

**POP Operation :-** (highlighted)
- Before deleting the element from the stack, we check whether the stack is empty.
- If we try to delete the element from empty stack, then underflow condition occurs.
- first access the element which is pointed by top.
- once the top operation is performed, top is decremented by 1 i.e. top = top -1.

Diagram. Four stack boxes from left to right, with curved arrows pointing left:
1. Holds [10, 20, 30], labels `top=1` and "pop=30".
2. Holds [10, 20], labels `top=-1` and "pop=20".
3. Holds [10], labels `top=-1` and "pop=10".
4. Empty, label `top=-1`, caption "empty".

### PDF p.44 (notebook p.43)

**Applications of Stack :-**
1). **Recursion** :- The recursion means that the function is calling itself again. To maintain the previous states, the compiler creates a system stack in which all previous records of function are maintained.
2). **DFS (Depth first search)** :- This search is implemented on a graph, graph uses stack d.s.
3). **Backtracking** :- If we have to create a path to solve maze problem, If we are moving in particular path and we realise that we come on the wrongway. In order to come at begining of the path to create a new path, we use stack d.s.
4). **memory management** :- The stack manages the memory. The memory is assign in the contiguous memory blocks.

The page has two columns. Left column:
```text
Algo :- push operation :-
begin
  if top = n then stack full
  top = top +1
  stack( top) : = item ;
end
```
Time complexity : O(1)

Right column:
```text
pop operation :-
begin
  if top = 0 then empty;
  item : = stack (top);
  top = top - 1 ;
end.
```
Time complexity : O(1)

### PDF p.45 (notebook p.44)

**Queue** :- (highlighted) A queue can be defined as ordered list which enables insert operations to be performed at one end called REAR and delete operations to be performed at another end called FRONT.
- Queue can be referred as to be first In first Out list.

Diagram: a horizontal array of 5 cells. "Enqueue (Insertion)" arrow enters at the right end, where "Rear ↑" sits under the last cell. "Dequeue (Deletion)" arrow leaves from the left end, where "front ↑" sits under the first cell.

**Complexity of queue :-**

| | Average: Access | Search | Deletion | Insertion | Space comp. worst |
|---|---|---|---|---|---|
| Queue | θ(n) | θ(n) | θ(1) | θ(1) | O(n) |

| | Worst: Access | Search | Insertion | Deletion |
|---|---|---|---|---|
| Queue | O(n) | O(n) | O(1) | O(1) |

**Operations on queue :-**
1). **Enqueue** :- Enqueue is used to insert element at rear end of the queue. It returns void.
2). **Dequeue** :- dequeue operations performs the deletion from front end of queue. The deque operation can also be designed to void.

### PDF p.46 (notebook p.45)

3). **peek** :- This returns, element which is pointed by front pointer in the queue but does not delete it.
4). **queue overflow (is full)** :- when queue is completely full, then it shows overflow condition.
5). **queue underflow (isempty)** :- When there is no element in the queue then it throws underflow condition.

**Types of queue :-** (highlighted)
1). **Linear queue** :- In linear queue, an insertion takes place from one end while deletion occurs from another end. It strictly follows FIFO rule. The linear queue can be represented, as shown :

Diagram: an array of 4 cells holding [10 | 20 | 30 | (empty)]. "front ↑" points to 10 and "Rear ↑" points to **20**.

The elements are inserted from rear end, and if we insert more elements in queue, then rear values gets incremented on every insertion.
drawback is using linear queue is : insertion is done only from rear end. The linear queue shows the overflow condition as rear is pointing to last element of the queue.

2). **circular queue** :- In circular queue, all nodes are represented as circular. It is similar to linear queue except that last element of the queue is connected to the first element. It is also known as ring buffer. as all ends are connected to another end.

### PDF p.47 (notebook p.46)

Diagram: a ring (annulus) split into about 11 slots. The outer index labels run clockwise from the top-right: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10. "← Front" points at index 0 and "Rear ←" points at index 6. The values inside the slots, clockwise from index 0, are 1, 3, 5, 7, 9, 11, 13. The other slots are empty.

Drawback of linear queue is overcomes in circular queue. If empty space is available, new element can be added in empty space by simply incrementing value of rear.

3). **Priority queue** :- The queue in which each element has some priority associated with it. Based on priority of the element, elements are aranged in a priority queue. If elements occur with same priority, then they are served according to FIFO principle.

\* **Array representation of queue :-** (highlighted)
Diagram: array [H | E | L | L | O] with indices 0 1 2 3 4. "Front ↑" is at 0 and "Rear ↑" is at 4. Caption: (fig : queue after inserting an element).

### PDF p.48 (notebook p.47)

After deleting element, value of front will increase from -1 to 0, the queue will look like :

Diagram: array [ (empty) | E | L | L | O ] with indices 0 1 2 3 4. "front ↑" is at 1 and "rear ↑" is at 4. Caption: (fig : queue after deleting an element).

**Algorithm to insert any element in a queue :-**
check if queue is already full by comparing rear to max -1.
```text
Algo: step 1 :- IF REAR = MAX -1
                write OVERFLOW
                Go to step     [END OF IF]
      step 2 :- IF FRONT = -1 and REAR = -1
                SET FRONT = REAR = 0
                ELSE
                SET REAR = REAR +1    [END OF IF].
      step 3 :- SET QUEUE [REAR] = NUM
      step 4 :- EXIT.
```

**Algorithm to delete an element from queue :-**
```text
Algo: step 1 :- IF FRONT = -1 or FRONT > REAR
                 write UNDERFLOW
                 ELSE
                 SET VAL = QUEUE [FRONT]
                 SET FRONT = FRONT +1
                 [END OF IF]
      step 2 :- EXIT.
```

### PDF p.49 (notebook p.48)

**Tree :-** (highlighted) We read data structure, like an array, linked list, stack and queue in which all elements are arranged in a sequential manner.
A tree is one of the data structures that represents hierarchical data.
**defination** :- A tree is a data structure defined as collection of objects or entities known as nodes that are linked together to represent or simulate hierarchy.
- A tree is a non-linear data structure because it does not store in a sequential manner. It is a hierarchical structure as elements in tree are arranged in multiple levels.
- In tree data structure topmost node is called as root node. Each node contains some data, & data can be of any type.
- Each node contains some data & link or reference of other nodes that can be called children.

Diagram: root **1** (labelled "← rootnode") has children 2 and 3. Node 2 has children 4, 5 and 6. Node 3 has children 7 and 8.

**Some Basic terms of tree :-** (highlighted)
1). **link** :- each node is labeled with some number. each array shown in fig is known as link between two nodes.
2). **Root** :- The root node is topmost node in tree hierarchy. root node is one that doesn't have any parent. If node is directly linked to some other

### PDF p.50 (notebook p.49)

node, then it would be called a parent-child relationship.
3). **child node** :- If the node is a descendant of any node, then node is called as child node.
4). **parent** :- If node contains any sub-node, then node is said to be parent of that sub-node.
5). **sibling** :- The nodes that have same parents are called siblings.
6). **leaf node** :- node which doesn't have any child node, a leaf a bottom-most node of tree.
7). **ancestor node** :- It is any predessor node on a path from root to that node. In the given fig, 1, 2, 5 are ancestors of node 10.
8). **Descendant** :- The immediate successor of given node is known as descendant of a node.

\* **Properties of tree data structures :-** (highlighted)
1). **Recursive data structure** :- Tree is also known as recursive data structure. Recursion means reducing something in a self-similar manner.
2). **Number of edges** :- If there are (n) nodes, then there would be (n-1) edges. each node, except root node, will have atleast one incoming link known as an edge.
3). **Depth of node x** :- It can be defined as length of path from root to node x. one edge contributes one unit length in the path, depth can be defined as no. of edges between root node and node (x). The root node has depth 0.
4). **Height of node x** :- It is defined as longest path from node x to leaf node.

### PDF p.51 (notebook p.50)

**Implementation of tree :-**
The tree data structure can be created nodes dynamically with help of pointers. The tree in memory can be represented as shown :

Diagram: a node with three cells [left | DATA | Right].
- The left cell's arrow goes to a node [ (empty) | B | x ].
- The Right cell's arrow goes to a node [ (empty) | C | (empty) ].

```c
Struct node
 {
  int data ;
 struct node *left ;
 struct node *right ;
 }
```
(The first word may look like "Struct" with a capital "S", but this reading is uncertain: the writer's lowercase s often looks like a capital. There is no `;` after the closing brace.) <!-- corrected: capital S in "Struct" is an uncertain reading, not a fact (see P3-38) -->

The above structure can only be defined for binary trees because binary tree can have utmost two children, and generic trees.

**Application of trees :-**
1). **storing naturally hierarchical data** :- File system, stored on disc drive, file and folder are in form of naturally heirarchical data and store in form of trees.
2). **organize data** :- It is used to organize data for efficient insertion, deletion and searching.
3). **Trie** :- It is special kind of tree that is used to store dictionary. It is fast and efficient way for dynamic spell checking.
4). **Heap** :- It is also a tree data structure implemented using arrays. It is used to implement priority queues.

### PDF p.52 (notebook p.51)

**Types of Tree data structure :-** (highlighted)
1). **General Type** :- In a general tree, a node can have either 0 or maximum n number of nodes. There is no restrictions imposed on the degree of node (number of nodes that a node can contain). The topmost node in a general tree is known as root node. The children of parent node are known as subtree.

Diagram: root **A** has children B, F and J. B has children C, D and E. F has children G and H. J has children K, L, M and N.

There can be n number of subtrees in general tree. In general tree, subtrees are unordered as nodes in subtree cannot be ordered.
Every non empty tree has a downward edge, and these edges are connected to nodes known as child nodes. The nodes that have same parent are known as siblings.

2). **Binary Tree** :- Binary tree means that the node can have maximum two children.
Diagram: 1 has children 2 and 3, and 2 has children 5 and 6. Red note: "← given tree is a binary tree because – each node contains atmost two children."

### PDF p.53 (notebook p.52)

**logical representation of above tree :-**
In above tree, node 1 contains two pointers i.e. left and right pointer pointing to left and right node respectively.

Diagram:
- Node [ | 1 | ]: its left pointer goes to [ | 2 | ] and its right pointer goes to [x | 3 | x].
- Node 2: its left pointer goes to [x | 5 | x] and its right pointer goes to [x | 6 | x].

The nodes 3, 5 and 6 are leaf nodes, so all these nodes contains NULL pointer on both left and right parts.

**Properties of Binary tree :-**
- At each level of i, the maximum number of nodes is 2^i.
- The height of tree is longest path from root node to leaf node. In general, maximum number of nodes possible at height h is (2^0 + 2^1 + 2^2 + … 2^h)
- The minimum number of nodes possible at height h is equal to h+1.
- If number of nodes is minimum, then height of tree would be maximum.
- minimum height can be computed as : h = log₂(n+1) − 1
- maximum height can be computed as : h = n − 1.

### PDF p.54 (notebook p.53)

**Types of Binary tree :-** (highlighted)
1). **full / proper / strict Binary tree :-**
If each node contains either 0 or two children. The tree in which each node must contain 2 children except left nodes.
Example :- A has children B and C, and B has children D and E.

Properties :-
- maximum number of nodes : 2^(h+1) − 1.
- minimum number of nodes : 2 * h − 1
- minimum height  log₂(n+1) − 1
- maximum height  h = (n+1)/2

2). **complete Binary Tree :-**
The tree in which all nodes are completely filled except the last level. In complete Binary tree nodes should be added from left.
Example :- 10 has children 20 and 30. 20 has children 40 and 50, and 30 has children 60 and 70. 40 has a left child 80.
