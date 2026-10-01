/*
 * dsa_tree.h — дерева (розділи 12–13 конспекту): BST, AVL, обходи, бінарна купа.
 */
#ifndef DSA_TREE_H
#define DSA_TREE_H

#include <stddef.h>
#include <stdint.h>

typedef struct tnode {
    int key;
    int height;              /* використовується AVL; для BST = висота піддерева */
    struct tnode *left, *right;
} tnode;

/* Лічильники для експериментів. */
typedef struct {
    uint64_t rotations;      /* одиничні повороти AVL (подвійний = 2) */
    uint64_t visited;        /* відвідані вузли під час пошуку */
} tree_stats;
extern tree_stats g_tree;

/* ---------- BST без балансування ---------- */
tnode *bst_insert(tnode *root, int key, int *ok);   /* ітеративно (немає рекурсії глибини n) */
tnode *bst_search(tnode *root, int key);
tnode *bst_delete(tnode *root, int key);           /* 3 випадки: 0, 1, 2 дитини (in-order наступник) */
void   tree_free(tnode *root);
int    tree_height(const tnode *root);             /* висота порожнього = 0, листа = 1 (кількість рівнів) */
size_t tree_size(const tnode *root);
int    bst_is_valid(const tnode *root);            /* перевіряє BST-властивість по діапазону, а не тільки дітей */

/* Обмеження: tree_height, tree_size, bst_is_valid і рекурсивні обходи мають глибину рекурсії = висоті
 * дерева. Для вироджених BST з сотнями тисяч вузлів це переповнює стек (виміряно рев'юером: ланцюжок
 * з 3M вузлів → SIGSEGV). Для AVL висота <= 1.44 log2 n — безпечно. Ітеративні: tree_levelorder,
 * tree_inorder_iterative, tree_free, bst_insert/search/delete. */

/* ---------- обходи: записують ключі у out, повертають кількість ---------- */
size_t tree_inorder(const tnode *root, int *out);
size_t tree_preorder(const tnode *root, int *out);
size_t tree_postorder(const tnode *root, int *out);
size_t tree_levelorder(const tnode *root, int *out, size_t cap_nodes);
size_t tree_inorder_iterative(const tnode *root, int *out, size_t cap_nodes);

/* ---------- AVL ---------- */
tnode *avl_insert(tnode *root, int key);
tnode *avl_delete(tnode *root, int key);
int    avl_is_balanced(const tnode *root);         /* |bf| <= 1 всюди і коректні height */

/* ---------- бінарна мін-купа (пріоритетна черга) ---------- */
typedef struct {
    int *a;
    size_t n, cap;
} minheap;

int  heap_init(minheap *h, size_t cap);
void heap_free(minheap *h);
int  heap_push(minheap *h, int v);   /* O(log n) */
int  heap_pop(minheap *h, int *out); /* O(log n) */
int  heap_peek(const minheap *h, int *out);
int  heap_build(minheap *h, const int *src, size_t n); /* Флойд, O(n); -1 = нема пам'яті */
int  heap_is_valid(const minheap *h);

#endif
