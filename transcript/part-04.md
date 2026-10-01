# DSA Handwritten Notes (Topper World): Part 04, PDF pages 55 to 72

> Verbatim transcription of the handwritten DSA notebook (English, as written), PDF pages 55–72.
> Original errors are intentionally preserved; only transcription mistakes were fixed (each marked with `<!-- corrected: ... -->`).
> For the list of errors in the notes themselves see [../errata/ERRATA-full.md](../errata/ERRATA-full.md).


### PDF p.55 (notebook p.54)

(Continuation of "2) Complete Binary Tree" from notebook p.53.)

**properties :-**
- maximum number of nodes → 2^(h+1) − 1.
- minimum number of nodes → 2^h
- minimum height → log₂(n+1) − 1.

**3). Perfect Binary tree :-**
A tree in which all the internal nodes have 2 children. and all leaf nodes are at the same level.

Example :-
Diagram: a perfect binary tree with 7 nodes. Root 1. Left child 2, right child 3. 2's children are 4 (left) and 5 (right). 3's children are 6 (left) and 7 (right).

*Note :-* All the perfect Binary trees are complete binary trees as well as the full Binary trees as But, vice versa is not true, all complete binary trees and full binary trees are the perfect Binary trees

**4). Balanced Binary Tree :-** (highlighted)
The balanced binary tree is a tree in which both left and right trees by almost 1.

Diagram: root 4. Left child 2, right child 5. 2's children are 1 (left) and 3 (right). 5 has only a right child, 6.

above tree is balance : diff betⁿ left subtree & right S.T. is zero

### PDF p.56 (notebook p.55)

**Binary Tree Implementation :-**
```c
struct node
{
  int data;
  struct node *left, *right;
}
```
(There is no semicolon after the closing brace.)

**Tree Traversal :-**
The process of visiting nodes is called as tree traversal.
There are three types of traversals used to visit a node :
1). Inorder Traversal
2). preorder Traversal
3). postorder Traversal.

**3). Binary Search Tree :-** (highlighted)
defn :- Binary search tree can be defined as a class of binary Trees, in which a nodes are arranged in a specific order. also called as ordered Binary Tree.
- similarly value of all nodes in right subtree is greater than or equal to value of root.

Diagram (labelled "← Root node" at 30): root 30. Left child 15, right child 60. 15's children are 7 (left) and 22 (right). 22's children are 17 (left) and 27 (right). 60's children are 45 (left) and 75 (right).

### PDF p.57 (notebook p.56)

Example : create binary search tree using following data elements :-
43, 10, 79, 90, 12, 54, 11, 9, 50.

→ 1). Insert 43 into tree as root of tree.
2). Read next element, if it is lesser root node element, insert it as root of left sub-tree.
3). otherwise, insert it as root of right of right subtree.

Diagrams (step headings in red):
- step 1: 43.
- step 2: 43 with left child 10.
- step 3: 43 with left child 10 and right child 79.
- step 4: as in step 3, plus 90 as the right child of 79.
- step 5: as in step 4, plus 12 as the right child of 10.
- step 6: as in step 5, plus 54 as the left child of 79.
- step 7: as in step 6, plus 11 as the left child of 12.
- step 8: as in step 7, plus 9 as the left child of 10.
- step 9: as in step 8, plus 50 as the left child of 54. Final tree: 43 has children 10 and 79. 10 has children 9 and 12. 12 has left child 11. 79 has children 54 and 90. 54 has left child 50.

### PDF p.58 (notebook p.57)

**Operations on Binary Search Tree (BST) :-** (highlighted)

| Sr.No. | Operation | Description |
|---|---|---|
| 1). | searching in BST | Finding location of some specific element in a Binary search Tree |
| 2). | Insertion in BST | Adding a new element to the binary search tree at appropriate location so that property of BST do not violate. |
| 3). | Deletion in BST. | Deleting some specific node from a BST, However, there there can be various cases in deletion depending upon number of children, node have. |

**4). AVL Tree :-** AVL Tree is invented by GM Adelson-Velsky and EM Landis in 1962. The tree is named as AVL in honour of its inventors.
AVL tree is defined as height balanced binary search tree in which each node is associated with a balance factor which is calculated by subtracting the height of its R.subtree from its left subtree.

Balance factor (k) = height(left(k)) − height(right(k))

- If balance factor of any node is 1, it means that left sub-tree is one level higher than right subtree.

### PDF p.59 (notebook p.58)

- If balance factor of any node is 0, it means that left sub-tree and right sub-tree contain equal height.
- If balance factor of any node is −1, it means that left sub-tree is one level lower than right subtree.

Example :-
Diagram: root 100, annotated "3−2=1". Left child 50, annotated "1−2=1" (no minus sign visible before the result). Right child 150, annotated "1−1=0". 50's children are 25 (annotated 0) and 75 (annotated "1−1=0"). 75's children are 65 (0) and 85 (0). 150's children are 125 (0) and 175 (0). <!-- corrected: the page shows "1−2=1", not "1−2=−1" (see P4-NEW-1) -->

Here we see that, balance factor associated with each node is between −1 and +1.
∴ It is an example of AVL tree.

**complexity :-**

| Algorithm | Average case | Worst case |
|---|---|---|
| space | o(n) | o(n) |
| search | o(log n) | o(log n) |
| Insert | o(log n) | o(log n) |
| Delete | o(log n) | o(log n) |

Why AVL Tree? → AVL tree controls height of binary search tree by not letting it to be skewed. The time taken by all operations in BST is o(h). However it will be extended to o(n) if BST became

### PDF p.60 (notebook p.59)

skewed (worst case). By limiting this height to log n, AVL tree imposes an upper bound on each operation to be O(log n), where n is number of nodes.

**Operations on AVL Tree :-**

| Sr.No | Operation | Description. |
|---|---|---|
| 1). | Insertion | Insertion is performed in same way it performed in BST. However, it may lead to violation in the AVL tree property and so tree may need balancing. and tree can be balanced by rotation. |
| 2). | Deletion | Deletion is also same way performed as BST It can be also disturb balance of tree, so various types of rotations are used to rebalance tree. |

**AVL Rotations :-**
We perform rotations in AVL tree only in case if Balance Factor is other than −1, 0 and 1.
There are Basically four types of rotations which are as follows :

### PDF p.61 (notebook p.60)

1). L−L rotation :- Inserted node is in the left subtree of left subtree of A.
2). R-R rotation :- Inserted node is in the right subtree of right subtree of A.
3). L-R rotation :- Inserted node is in right subtree of left subtree of A.
4). R−L Rotation :- Inserted node is in the left subtree of right subtree of A.

**5). B Tree :-** B Tree is specialized m-way tree that can be widely used for disk access. A B-tree of order m can have at most m−1 keys and m children.
Properties :-
1. Every node in B-Tree contains at most m children.
2. Every node in B-Tree except root node and leaf node contain at least m/2 children.
3. The root nodes must have at least 2 nodes.
4. All leaf nodes must be at the same level.

Example :-
Diagram: a B-tree; the root is drawn with 3 slots and the internal nodes with 4 slots. Root [ _ | 60 | _ ]. Its left child is [ _ | 29 | 32 | _ ] and its right child is [ _ | 90 | 98 | _ ]. The children of [29, 32] are leaves [10 | 23], [30 | 31] and [45 | 58]. The children of [90, 98] are leaves [70 | 85], [93 | 96] and [101]. <!-- corrected: "3 key slots per node" was not exact: root has 3 slots, internal nodes [ _ |29|32| _ ] have 4 -->

### PDF p.62 (notebook p.61)

**Operations :-** (highlighted)
1). searching :- The searching in B tree is similar to searching in Binary tree. for example, we search for an item 49 in following B Tree. The process will be :
① compare item 49 with root node 78. since 49 < 78 hence, move its left sub-tree.
② since, 40 < 49 < 56, traverse right subtree of 40.
③ 49 > 45, move to right compare 49.
④ match found, return.
Searching in B tree depends upon height of the tree. The search algorithm takes O(log n) time to search any element in B tree.

2). Inserting :- Insertion are done at leaf node level. The following algorithm needs to be followed in order to insert an item into B tree.
① Traverse B tree in order to find appropriate leaf node at which node can be inserted.
② If leaf node contains less than m−1 keys then insert element in increasing order.
③ Else, if leaf node contains m−1 keys, then follow following steps:
- Insert new element in increasing order of elements.
- split node into two nodes at median.
- Push median element upto its parent node.
- If parent node also contain m−1 number of keys, then split it too by steps.

(A small "10" appears in a box in the left margin.)

**Application of B tree :-** (highlighted)
- B tree is used to index data and provides fast access to actual data stored on disks since, the

### PDF p.63 (notebook p.62)

access to value stored in a large database that is stored on a disk is a very time consuming process.
Searching an un-indexed and unsorted database containing n key values needs O(n) running time.

**6). B + Tree :-**
B+tree is an extension of B tree which allows efficient insertion, deletion and search operations.
The leaf nodes of B+ tree are linked together in form of the singly linked list to make search queries more efficient.
Advantages of B+ tree :-
1). Records can be fetched in equal number of disk accesses.
2). Height of tree remains balanced and less as compare to B tree.
3). We can access data stored in B+ tree sequentially as well as directly.
4). Keys are used for indexing.

**Graph :-** (highlighted)
A graph can be defined as group of vertices and edges that are used to connect these vertices.
Defination :-
A graph G can be defined as an ordered set G(V, E) where V(G) represents set of vertices and E(G) represents set of edges

### PDF p.64 (notebook p.63)

which are used to connect these vertices.

**Directed and Undirected Graph :-**
A graph can be directed or undirected. However, in an undirected graph, edges are not associated with directions with them.

Diagram (Fig : Undirected graph): vertices A, B, C on top and D, E below. Edges: A–B, B–C, A–D, B–D, C–E, D–E.

As above figure, edges are not attached with any of the directions.

Diagram (Fig : directed graph): same layout. Edges: A→B, B→C, B→D, C→E, E→D, D→A.

In above figure, directed graph edges form an ordered pair.

**Graph Terminology :-** (highlighted)
1). Path :- A path can be defined as sequence of nodes that are followed in order to reach some terminal node v from initial node U.
2). closed path :- A path will be called as closed if initial node is same as terminal node. V₀ = V_N

### PDF p.65 (notebook p.64)

3). simple path :- If all nodes of graph are distinct with an exception V₀ = V_N, then such path P is called as closed simple path.
4). Cycle :- A cycle is a path which has no repeated edges or vertices except first and last vertices.
5). Connected graph :- A graph in which some path exists between every two vertices (u, v) in V. There are no isolated nodes in connected graph.
6). Complete graph :- A graph in which every node is connected with all other nodes. A complete graph contain n(n−1)/2 edges where n is number of nodes in graph.
7). Weighted graph :- In this graph each node is assigned with some data such as length or width. The weight of an edge e can be given as w(e) which must be positive (+) value indicating cost of traversing edge.
8). Diagraph :- A diagraph is directed graph in which each edge is associated with some direction and traversing can be done only in specified direction.
9). Loop :- An edge that is associated with the similar end points can be called as loop.
10). Adjacent Nodes :- If two nodes u and v are connected via an edge e, then nodes u and v

### PDF p.66 (notebook p.65)

are called as neighbours or adjacent nodes.
11). Degree of a Node :- A degree of a node is a number of edges that are connected with that node. A node with degree 0 is called isolated.

**Graph Representation :-** (highlighted)
We simply mean, technique which is to be used to in order to store some graph into the computers memory.
① Sequential Representation :- In this we use adjacency matrix to store mapping represented by vertices and edges. A graph having n vertices, will have a dimension n×n.
An entry Mij in adjacency matrix representation of an ~~mat~~ undirected graph G will be 1 if there exists an edge between vi and vj.
An undirected graph and its adjacency matrix representation is shown in following :

Diagram (fig : Undirected graph): edges A–B, B–C, A–D, B–D, C–E, D–E.

Fig: Adjacency matrix:

|   | A | B | C | D | E |
|---|---|---|---|---|---|
| A | 0 | 1 | 0 | 1 | 0 |
| B | 1 | 0 | 1 | 1 | 0 |
| C | 0 | 1 | 0 | 0 | 1 |
| D | 1 | 1 | 0 | 0 | 1 |
| E | 0 | 0 | 1 | 1 | 0 |

In above figure, we can see mapping among vertices (A, B, C, D, E) is represented by using adjacency matrix which is also shown in fig.

### PDF p.67 (notebook p.66)

A directed graph and its adjacency matrix representation is shown in figure :

Diagram (Fig : Directed Graph): A→B, B→C, B→D, C→E, E→D, D→A.

Fig: Adjacency matrix:

|   | A | B | C | D | E |
|---|---|---|---|---|---|
| A | 0 | 1 | 0 | 0 | 0 |
| B | 0 | 0 | 1 | 1 | 0 |
| C | 0 | 0 | 0 | 0 | 1 |
| D | 1 | 0 | 0 | 0 | 0 |
| E | 0 | 0 | 0 | 1 | 0 |

Representation of weighted directed graph is different. Instead of filling entry by 1, non zero entries of adjacency matrix are represented by weight of respective edges.

Diagram (Fig : weighted directed graph): A→B (4), B→C (2), B→D (1), C→E (8), E→D (10), D→A (5).

Fig: Adjancy matrix:

|   | A | B | C | D | E |
|---|---|---|---|---|---|
| A | 0 | 4 | 0 | 0 | 0 |
| B | 0 | 0 | 2 | 1 | 0 |
| C | 0 | 0 | 0 | 0 | 8 |
| D | 5 | 0 | 0 | 0 | 0 |
| E | 0 | 0 | 0 | 10 | 0 |

② linked representation :-
Diagram (Fig : undirected graph): edges A–B, B–C, A–D, B–D, C–E, D–E.
Fig : Adjacency list (head node → list nodes, X = NULL):
- A → B → D X
- B → A → D → C X
- C → B → E X
- D → A → B → E X
- E → D → C X

### PDF p.68 (notebook p.67)

An adjacency list is maintained for each node present in graph which stores node value and a pointer to next adjacent node to respective node.

Diagram (fig : Directed graph): A→B, B→C, B→D, C→E, E→D, D→A.
Fig: Adjancy list:
- A → B X
- B → C → D X
- C → E X
- D → A X
- E → D X

In directed graph, sum of lengths of all the adjancy lists is equal to the number of edges present in the graph.

**Graph Traversal Algorithm :-** (highlighted)
In this tutorial we will learn all techniques by using which, we can traverse all the vertices of the graph. Traversing means examining all nodes and vertices of graph. There are two standard methods by using which, we can traverse graphs.
- Breadth first search
- Depth first search

① Breadth first search (BFS) algorithm :-
Breadth first search is a graph traversal algorithm that starts traversing graph from root node and explores all the neighbouring nodes.
Then, it selects nearest node and explore all unexplored nodes. The algorithm follows same process for each of nearest node until it finds goal.

### PDF p.69 (notebook p.68)

Algorithm :-
- step 1: SET STATUS = 1 (ready state) for each node in G.
- step 2: Enqueue starting node A & set its STATUS = 2 (waiting state)
- step 3: Repeat steps 4 and 5 until QUEUE is empty.
- step 4: Dequeue a node N. Process it & set its STATUS = 3
- step 5: Enqueue all neighbours of N that are in ready state (whose STATUS = 1) & set (STATUS = 2) [END OF LOOP].
- step 6: EXIT.

Example :-
Consider graph G shown in following image, calculate minimum path P from node A to node E. Given that each edge has a length of 1.

Diagram (directed): A→B, B→C, C→G, C→E, G→E, E→F, D→F, F→A, A→D.

Adjacency lists :
- A : B, D
- B : C, F
- C : E, G
- G : E
- E : B, F
- F : A
- D : F.

Solution :-
minimum Path P can be found by applying Breadth first search algorithm that will begin at node A and will end at E.
A → B → C → E

### PDF p.70 (notebook p.69)

**Depth First Search Algorithm :-** (highlighted)
DFS algorithm starts with initial node of graph G, & goes to deeper & deeper until we find goal node / node which has no children.
The data structure used in DFS is stack.
Algorithm :-
- step 1: SET STATUS = 1 (ready state) for each node in G
- step 2: Push starting node A on stack & set its STATUS = 2 (waiting state).
- step 3: Repeat steps 4 and 5 until stack is empty.
- step 4: Pop top node N. Process it & set its STATUS = 3.
- step 5: push on stack all neighbours of N that are in ready state (whose STATUS = 1) and set their STATUS = 2 (waiting state) [END OF LOOP].
- step 6: EXIT.

**Spanning Tree :-** (highlighted)
If we have a graph containing V vertices and E edges, then graph can be represented as: G(V, E). If we create spanning tree from above graph, then spanning tree would have same number of vertices as the graph, but vertices are not equal.
edges (Spanning tree) = no. of edge (in graph) − 1.

Example :-
- Graph: 1–2 (1), 1–5 (3), 2–3 (4), 5–4 (2), 3–4 (5). This is a 5-cycle.
- → Spanning trees (total weights circled in red):
  - (i) 1–2 (1), 2–3 (labelled **1**), 3–4 (5), 5–4 (2). Total circled **12**.
  - (ii) 1–5 (3), 2–3 (4), 3–4 (5), 5–4 (2). Total circled **14**.
  - (iii) 1–2 (1), 1–5 (3), 5–(node labelled **9**) (2), 3–(node labelled **9**) (5). Total circled **11**.

### PDF p.71 (notebook p.70)

**Minimum Spanning Trees :-**
The minimum spanning tree is a tree whose sum of edge weights is minimum.

Diagram: 1–2 (1), 1–5 (3), 2–3 (4), 5–4 (2). Total circled 10. The 3–4 edge is not included.

In above tree, total edge weight is less than above spanning trees, therefore a minimum spanning tree is a tree which is having an edge weight i.e. 10.

**Properties of spanning tree :-** (highlighted)
- A connected graph can contain more than one spanning tree.
- All possible spanning trees that can be created from given graph G would have same number of vertices in given graph minus 1.
- Spanning tree does not contain any cycle. let's understand this property through an example.
- spanning tree cannot be disconnected. If we remove one more edge from any of above spanning trees as.
- If two / more edges have same edge weight, then there would be more than two minimum spanning tree. If each edge has a distinct weight, then there will be only one / unique spanning tree.

### PDF p.72 (notebook p.71)

**Applications of Spanning tree :-**
1). Building a network :- suppose there are many routers in network connected to each other, so there might be a possibility that it forms a loop.
2). clustering :- clustering means that grouping set of objects in such way that similar objects belong to same group than to different group. our goal is to divide the n objects into k groups such that distance between different groups gets maximised.

**Searching :-** (highlighted)
searching is a process of finding some particular element in list. If the element is present in the list, then process is called successful and process returns location of that element, otherwise search is called unsuccessful.
There are two methods widely used as below:
- Linear search
- Binary search

1). linear search :-
linear search is a simplest sequential search algorithm and often called sequential search. In this type of searching, we simply traverse the list completely and match each element of list with item whose location is to be found.
linear search is mostly used to search an unordered list in which items are not sorted.
The algorithm is given as follows :
