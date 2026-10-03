/*
 * dsa_tree.h — trees (sections 12-13 of the notes): BST, AVL, traversals, binary heap.
 */
#ifndef DSA_TREE_H
#define DSA_TREE_H

#include <stddef.h>
#include <stdint.h>

typedef struct tnode {
    int key;
    int height;              /* used by AVL; for BST = subtree height */
    struct tnode *left, *right;
} tnode;

/* Counters for experiments. */
typedef struct {
    uint64_t rotations;      /* single AVL rotations (double = 2) */
    uint64_t visited;        /* nodes visited during search */
} tree_stats;
extern tree_stats g_tree;

/* ---------- unbalanced BST ---------- */
tnode *bst_insert(tnode *root, int key, int *ok);   /* iterative (no recursion of depth n) */
tnode *bst_search(tnode *root, int key);
tnode *bst_delete(tnode *root, int key);           /* 3 cases: 0, 1, 2 children (in-order successor) */
void   tree_free(tnode *root);
int    tree_height(const tnode *root);             /* height of empty = 0, of a leaf = 1 (number of levels) */
size_t tree_size(const tnode *root);
int    bst_is_valid(const tnode *root);            /* checks the BST property by range, not just children */

/* Limitation: tree_height, tree_size, bst_is_valid and the recursive traversals have recursion depth equal
 * to the tree height. For degenerate BSTs with hundreds of thousands of nodes this overflows the stack
 * (measured in review: a chain of 3M nodes → SIGSEGV). For AVL height <= 1.44 log2 n - safe. Iterative: tree_levelorder,
 * tree_inorder_iterative, tree_free, bst_insert/search/delete. */

/* ---------- traversals: write keys to out, return the count ---------- */
size_t tree_inorder(const tnode *root, int *out);
size_t tree_preorder(const tnode *root, int *out);
size_t tree_postorder(const tnode *root, int *out);
size_t tree_levelorder(const tnode *root, int *out, size_t cap_nodes);
size_t tree_inorder_iterative(const tnode *root, int *out, size_t cap_nodes);

/* ---------- AVL ---------- */
tnode *avl_insert(tnode *root, int key);
tnode *avl_delete(tnode *root, int key);
int    avl_is_balanced(const tnode *root);         /* |bf| <= 1 everywhere and correct heights */

/* ---------- binary min-heap (priority queue) ---------- */
typedef struct {
    int *a;
    size_t n, cap;
} minheap;

int  heap_init(minheap *h, size_t cap);
void heap_free(minheap *h);
int  heap_push(minheap *h, int v);   /* O(log n) */
int  heap_pop(minheap *h, int *out); /* O(log n) */
int  heap_peek(const minheap *h, int *out);
int  heap_build(minheap *h, const int *src, size_t n); /* Floyd, O(n); -1 = out of memory */
int  heap_is_valid(const minheap *h);

#endif
