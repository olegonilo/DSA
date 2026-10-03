#include "dsa_common.h"
#include "dsa_graph.h"
#include "dsa_tree.h"
#include "minitest.h"

#include <stdlib.h>
#include <string.h>

static int eq(const int *a, const int *b, size_t n) { return memcmp(a, b, n * sizeof *a) == 0; }

static void test_bst_from_notes(void) {
    /* Example from the notes (notebook p.57): insert 43, 10, 79, 90, 12, 54, 11, 9, 50 */
    int keys[] = {43, 10, 79, 90, 12, 54, 11, 9, 50};
    tnode *r = NULL;
    for (int i = 0; i < 9; i++) r = bst_insert(r, keys[i], NULL);
    int out[16];
    size_t n = tree_preorder(r, out);
    int pre[] = {43, 10, 9, 12, 11, 79, 54, 50, 90};
    CHECK_EQ_INT(n, 9);
    CHECK(eq(out, pre, 9));
    n = tree_inorder(r, out);
    int in[] = {9, 10, 11, 12, 43, 50, 54, 79, 90};
    CHECK(eq(out, in, 9));
    tree_postorder(r, out);
    int post[] = {9, 11, 12, 10, 50, 54, 90, 79, 43};
    CHECK(eq(out, post, 9));
    tree_levelorder(r, out, 16);
    int lvl[] = {43, 10, 79, 9, 12, 54, 90, 11, 50};
    CHECK(eq(out, lvl, 9));
    tree_inorder_iterative(r, out, 16);
    CHECK(eq(out, in, 9));
    CHECK_EQ_INT(tree_height(r), 4);
    CHECK(bst_is_valid(r));

    r = bst_delete(r, 9);    /* leaf */
    r = bst_delete(r, 12);   /* one child */
    r = bst_delete(r, 43);   /* two children: root is replaced by 50 */
    CHECK_EQ_INT(r->key, 50);
    n = tree_inorder(r, out);
    int in2[] = {10, 11, 50, 54, 79, 90};
    CHECK_EQ_INT(n, 6);
    CHECK(eq(out, in2, 6));
    CHECK(bst_is_valid(r));
    tree_free(r);
}

static void test_bst_validity_trap(void) {
    /* A tree where EVERY parent is valid relative to its children, yet it is not a BST:
     *       10
     *      /  \
     *     5    15
     *         /
     *        6      <- 6 < 10 in the right subtree */
    tnode n6 = {6, 1, NULL, NULL}, n5 = {5, 1, NULL, NULL};
    tnode n15 = {15, 2, &n6, NULL}, n10 = {10, 3, &n5, &n15};
    CHECK(!bst_is_valid(&n10));
}

static void test_avl(void) {
    tnode *r = NULL;
    for (int i = 1; i <= 1023; i++) r = avl_insert(r, i);   /* sorted insertion - worst case for BST */
    CHECK(avl_is_balanced(r));
    CHECK_EQ_INT(tree_height(r), 10);                       /* 2^10 - 1 = 1023 => perfect tree */
    CHECK(bst_is_valid(r));
    for (int i = 1; i <= 1023; i += 2) r = avl_delete(r, i);
    CHECK(avl_is_balanced(r));
    CHECK_EQ_INT(tree_size(r), 511);
    CHECK(bst_is_valid(r));
    tree_free(r);

    /* Four rotation cases, 3 insertions each */
    int cases[4][3] = {{30, 20, 10}, {10, 20, 30}, {30, 10, 20}, {10, 30, 20}}; /* LL RR LR RL */
    uint64_t rot_expected[4] = {1, 1, 2, 2};
    for (int c = 0; c < 4; c++) {
        g_tree.rotations = 0;
        tnode *t = NULL;
        for (int i = 0; i < 3; i++) t = avl_insert(t, cases[c][i]);
        CHECK_EQ_INT(t->key, 20);
        CHECK_EQ_INT(g_tree.rotations, rot_expected[c]);
        tree_free(t);
    }
}

static void test_heap(void) {
    minheap h;
    heap_init(&h, 2);
    dsa_rng r;
    dsa_rng_seed(&r, 1);
    int a[5000];
    for (int i = 0; i < 5000; i++) { a[i] = (int)dsa_rng_below(&r, 1000); heap_push(&h, a[i]); }
    CHECK(heap_is_valid(&h));
    int prev = -1, v;
    for (int i = 0; i < 5000; i++) { CHECK_EQ_INT(heap_pop(&h, &v), 0); CHECK(v >= prev); prev = v; }
    CHECK_EQ_INT(heap_pop(&h, &v), -1);
    heap_build(&h, a, 5000);
    CHECK(heap_is_valid(&h));
    heap_free(&h);
}

/* Graph from the notes (notebook p.68, "BFS shortest path A→E"): check deterministic orders. */
static void test_graph(void) {
    /* 0-1, 0-2, 0-3, 1-3, 2-4, 3-4, 4-5 (undirected) */
    edge es[] = {{0, 1}, {0, 2}, {0, 3}, {1, 3}, {2, 4}, {3, 4}, {4, 5}};
    graph_csr g;
    graph_matrix m;
    CHECK_EQ_INT(gc_build(&g, 6, es, 7, 1), 0);
    CHECK_EQ_INT(gm_build(&m, 6, es, 7, 1), 0);
    int o1[6], o2[6], dist[6];
    CHECK_EQ_INT(bfs_csr(&g, 0, o1, dist), 6);
    int bfs[] = {0, 1, 2, 3, 4, 5};
    CHECK(eq(o1, bfs, 6));
    int d[] = {0, 1, 1, 1, 2, 3};
    CHECK(eq(dist, d, 6));
    bfs_matrix(&m, 0, o2);
    CHECK(eq(o1, o2, 6));

    int dfs[] = {0, 1, 3, 4, 2, 5};
    dfs_csr_recursive(&g, 0, o1);
    CHECK(eq(o1, dfs, 6));
    dfs_csr_iterative(&g, 0, o2);
    CHECK(eq(o2, dfs, 6));
    dfs_matrix(&m, 0, o2);
    CHECK(eq(o2, dfs, 6));

    int comp[6];
    CHECK_EQ_INT(connected_components(&g, comp), 1);
    gc_free(&g);
    gm_free(&m);

    edge dag[] = {{5, 2}, {5, 0}, {4, 0}, {4, 1}, {2, 3}, {3, 1}};
    CHECK_EQ_INT(gc_build(&g, 6, dag, 6, 0), 0);
    int topo[6];
    CHECK_EQ_INT(topo_sort_kahn(&g, topo), 6);
    int pos[6];
    for (int i = 0; i < 6; i++) pos[topo[i]] = i;
    for (int i = 0; i < 6; i++) CHECK(pos[dag[i].u] < pos[dag[i].v]);
    gc_free(&g);

    edge cyc[] = {{0, 1}, {1, 2}, {2, 0}};
    gc_build(&g, 3, cyc, 3, 0);
    CHECK_EQ_INT(topo_sort_kahn(&g, topo), -1);
    gc_free(&g);
}

static void test_inorder_cap(void) {
    /* Review: cap did not bound out and the internal stack → stack-buffer-overflow. */
    tnode *r = NULL;
    for (int i = 0; i < 10; i++) r = bst_insert(r, i * 7 % 10, NULL);
    int out[3];
    size_t k = tree_inorder_iterative(r, out, 3);
    CHECK_EQ_INT(k, 3);
    CHECK_EQ_INT(out[0], 0);
    CHECK_EQ_INT(out[2], 2);
    tree_free(r);
    r = NULL;
    for (int i = 10; i > 0; i--) r = bst_insert(r, i, NULL);          /* left chain of depth 10 */
    k = tree_inorder_iterative(r, out, 3);
    CHECK(k <= 3);
    tree_free(r);
}

int main(void) {
    puts("test_tree_graph");
    RUN(test_inorder_cap);
    RUN(test_bst_from_notes);
    RUN(test_bst_validity_trap);
    RUN(test_avl);
    RUN(test_heap);
    RUN(test_graph);
    return TEST_SUMMARY();
}
