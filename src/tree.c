#include "dsa_tree.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

tree_stats g_tree;

static tnode *tnew(int key) {
    tnode *n = malloc(sizeof *n);
    if (!n) return NULL;
    n->key = key;
    n->height = 1;
    n->left = n->right = NULL;
    return n;
}

/* ================================================================ BST */

tnode *bst_insert(tnode *root, int key, int *ok) {
    tnode **pp = &root;
    while (*pp) {
        if (key == (*pp)->key) { if (ok) *ok = 0; return root; } /* дублікати не вставляємо */
        pp = key < (*pp)->key ? &(*pp)->left : &(*pp)->right;
    }
    *pp = tnew(key);
    if (ok) *ok = *pp != NULL;
    return root;
}

tnode *bst_search(tnode *root, int key) {
    while (root) {
        g_tree.visited++;
        if (key == root->key) return root;
        root = key < root->key ? root->left : root->right;
    }
    return NULL;
}

tnode *bst_delete(tnode *root, int key) {
    tnode **pp = &root;
    while (*pp && (*pp)->key != key) pp = key < (*pp)->key ? &(*pp)->left : &(*pp)->right;
    tnode *d = *pp;
    if (!d) return root;
    if (d->left && d->right) {
        /* 2 дитини: копіюємо ключ in-order наступника (мінімум правого піддерева) і видаляємо його */
        tnode **sp = &d->right;
        while ((*sp)->left) sp = &(*sp)->left;
        tnode *s = *sp;
        d->key = s->key;
        *sp = s->right;
        free(s);
    } else {
        *pp = d->left ? d->left : d->right; /* 0 або 1 дитина */
        free(d);
    }
    return root;
}

void tree_free(tnode *root) {
    /* Ітеративно через "розгортання" лівих піддерев — без рекурсії глибини h. */
    while (root) {
        if (root->left) {
            tnode *l = root->left;
            root->left = l->right;
            l->right = root;
            root = l;
        } else {
            tnode *r = root->right;
            free(root);
            root = r;
        }
    }
}

int tree_height(const tnode *root) {
    if (!root) return 0;
    int l = tree_height(root->left), r = tree_height(root->right);
    return 1 + (l > r ? l : r);
}

size_t tree_size(const tnode *root) {
    return root ? 1 + tree_size(root->left) + tree_size(root->right) : 0;
}

static int valid_range(const tnode *n, int64_t lo, int64_t hi) {
    if (!n) return 1;
    if (n->key <= lo || n->key >= hi) return 0;
    return valid_range(n->left, lo, n->key) && valid_range(n->right, n->key, hi);
}
int bst_is_valid(const tnode *root) { return valid_range(root, (int64_t)INT_MIN - 1, (int64_t)INT_MAX + 1); }

/* ================================================================ traversals */

static size_t in_rec(const tnode *n, int *out, size_t k) {
    if (!n) return k;
    k = in_rec(n->left, out, k);
    out[k++] = n->key;
    return in_rec(n->right, out, k);
}
static size_t pre_rec(const tnode *n, int *out, size_t k) {
    if (!n) return k;
    out[k++] = n->key;
    k = pre_rec(n->left, out, k);
    return pre_rec(n->right, out, k);
}
static size_t post_rec(const tnode *n, int *out, size_t k) {
    if (!n) return k;
    k = post_rec(n->left, out, k);
    k = post_rec(n->right, out, k);
    out[k++] = n->key;
    return k;
}
size_t tree_inorder(const tnode *r, int *out) { return in_rec(r, out, 0); }
size_t tree_preorder(const tnode *r, int *out) { return pre_rec(r, out, 0); }
size_t tree_postorder(const tnode *r, int *out) { return post_rec(r, out, 0); }

size_t tree_levelorder(const tnode *root, int *out, size_t cap) {
    if (!root || cap == 0) return 0;
    const tnode **q = malloc(cap * sizeof *q);
    if (!q) return 0;
    size_t head = 0, tail = 0, k = 0;
    q[tail++] = root;
    while (head < tail) {
        const tnode *n = q[head++];
        out[k++] = n->key;
        if (n->left && tail < cap) q[tail++] = n->left;
        if (n->right && tail < cap) q[tail++] = n->right;
    }
    free(q);
    return k;
}

size_t tree_inorder_iterative(const tnode *root, int *out, size_t cap) {
    /* cap обмежує і out, і стек: стек містить лише вузли, ще не записані в out,
     * тож top + k <= кількість вузлів; зупиняємось, щойно k == cap або стек повний. */
    const tnode **st = malloc((cap ? cap : 1) * sizeof *st);
    if (!st) return 0;
    size_t top = 0, k = 0;
    const tnode *cur = root;
    while ((cur || top) && k < cap) {
        while (cur) {
            if (top == cap) { free(st); return k; } /* дерево глибше за cap */
            st[top++] = cur;
            cur = cur->left;
        }
        cur = st[--top];
        out[k++] = cur->key;
        cur = cur->right;
    }
    free(st);
    return k;
}

/* ================================================================ AVL */

static int h(const tnode *n) { return n ? n->height : 0; }
static void upd(tnode *n) { int a = h(n->left), b = h(n->right); n->height = 1 + (a > b ? a : b); }
static int bf(const tnode *n) { return h(n->left) - h(n->right); }

static tnode *rot_right(tnode *y) {
    tnode *x = y->left;
    y->left = x->right;
    x->right = y;
    upd(y); upd(x);
    g_tree.rotations++;
    return x;
}
static tnode *rot_left(tnode *x) {
    tnode *y = x->right;
    x->right = y->left;
    y->left = x;
    upd(x); upd(y);
    g_tree.rotations++;
    return y;
}

static tnode *rebalance(tnode *n) {
    upd(n);
    int b = bf(n);
    if (b > 1) {                                    /* ліве важче */
        if (bf(n->left) < 0) n->left = rot_left(n->left);   /* LR */
        return rot_right(n);                        /* LL */
    }
    if (b < -1) {                                   /* праве важче */
        if (bf(n->right) > 0) n->right = rot_right(n->right); /* RL */
        return rot_left(n);                         /* RR */
    }
    return n;
}

tnode *avl_insert(tnode *root, int key) {
    if (!root) return tnew(key);
    if (key < root->key) root->left = avl_insert(root->left, key);
    else if (key > root->key) root->right = avl_insert(root->right, key);
    else return root;
    return rebalance(root);
}

tnode *avl_delete(tnode *root, int key) {
    if (!root) return NULL;
    if (key < root->key) root->left = avl_delete(root->left, key);
    else if (key > root->key) root->right = avl_delete(root->right, key);
    else {
        if (!root->left || !root->right) {
            tnode *c = root->left ? root->left : root->right;
            free(root);
            return c;
        }
        tnode *s = root->right;
        while (s->left) s = s->left;
        root->key = s->key;
        root->right = avl_delete(root->right, s->key);
    }
    return rebalance(root);
}

int avl_is_balanced(const tnode *n) {
    if (!n) return 1;
    int a = h(n->left), b = h(n->right);
    if (n->height != 1 + (a > b ? a : b)) return 0;
    if (a - b > 1 || b - a > 1) return 0;
    return avl_is_balanced(n->left) && avl_is_balanced(n->right);
}

/* ================================================================ heap */

int heap_init(minheap *hp, size_t cap) {
    hp->a = malloc((cap ? cap : 1) * sizeof *hp->a);
    hp->n = 0;
    hp->cap = cap ? cap : 1;
    return hp->a ? 0 : -1;
}
void heap_free(minheap *hp) { free(hp->a); hp->a = NULL; hp->n = hp->cap = 0; }

static void sift_up(int *a, size_t i) {
    int v = a[i];
    while (i > 0) {
        size_t p = (i - 1) / 2;   /* 0-індексація: батько (i-1)/2, діти 2i+1, 2i+2 */
        if (a[p] <= v) break;
        a[i] = a[p];
        i = p;
    }
    a[i] = v;
}

static void sift_down_min(int *a, size_t i, size_t n) {
    int v = a[i];
    for (;;) {
        size_t c = 2 * i + 1;
        if (c >= n) break;
        if (c + 1 < n && a[c + 1] < a[c]) c++;
        if (v <= a[c]) break;
        a[i] = a[c];
        i = c;
    }
    a[i] = v;
}

int heap_push(minheap *hp, int v) {
    if (hp->n == hp->cap) {
        int *p = realloc(hp->a, hp->cap * 2 * sizeof *p);
        if (!p) return -1;
        hp->a = p;
        hp->cap *= 2;
    }
    hp->a[hp->n] = v;
    sift_up(hp->a, hp->n++);
    return 0;
}

int heap_pop(minheap *hp, int *out) {
    if (hp->n == 0) return -1;
    if (out) *out = hp->a[0];
    hp->a[0] = hp->a[--hp->n];
    if (hp->n) sift_down_min(hp->a, 0, hp->n);
    return 0;
}

int heap_peek(const minheap *hp, int *out) {
    if (hp->n == 0) return -1;
    *out = hp->a[0];
    return 0;
}

int heap_build(minheap *hp, const int *src, size_t n) {
    if (n > hp->cap) {
        int *p = realloc(hp->a, n * sizeof *p);
        if (!p) return -1;
        hp->a = p;
        hp->cap = n;
    }
    memcpy(hp->a, src, n * sizeof *src);
    hp->n = n;
    for (size_t i = n / 2; i-- > 0;) sift_down_min(hp->a, i, n);
    return 0;
}

int heap_is_valid(const minheap *hp) {
    for (size_t i = 1; i < hp->n; i++)
        if (hp->a[(i - 1) / 2] > hp->a[i]) return 0;
    return 1;
}
