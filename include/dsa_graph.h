/*
 * dsa_graph.h — графи (розділи 14–15 конспекту): представлення і обходи BFS/DFS.
 *
 * Два представлення, щоб порівняти їх експериментально:
 *   - матриця суміжності: Θ(V^2) пам'яті, перевірка ребра O(1), обхід Θ(V^2);
 *   - список суміжності у форматі CSR (compressed sparse row): Θ(V+E) пам'яті, обхід Θ(V+E).
 * Вершини нумеруються 0..V-1. Сусіди обходяться у порядку зростання номера,
 * щоб порядок обходу був детермінованим і збігався з ручним трасуванням.
 */
#ifndef DSA_GRAPH_H
#define DSA_GRAPH_H

#include <stddef.h>
#include <stdint.h>

typedef struct { int u, v; } edge;

typedef struct {
    int V;
    unsigned char *m;   /* V*V, m[u*V+v] = 1 якщо є ребро u->v */
} graph_matrix;

typedef struct {
    int V;
    size_t E;           /* кількість орієнтованих дуг (неорієнтоване ребро = 2 дуги) */
    size_t *off;        /* V+1 зсувів */
    int *adj;           /* E сусідів */
} graph_csr;

int  gm_build(graph_matrix *g, int V, const edge *es, size_t ne, int undirected);
void gm_free(graph_matrix *g);
int  gc_build(graph_csr *g, int V, const edge *es, size_t ne, int undirected);
void gc_free(graph_csr *g);

/* Обходи повертають кількість відвіданих вершин; order[] — порядок відвідування,
 * dist[] (BFS, може бути NULL) — кількість ребер від src, -1 якщо недосяжна. */
int bfs_csr(const graph_csr *g, int src, int *order, int *dist);
int dfs_csr_iterative(const graph_csr *g, int src, int *order);   /* явний стек, порядок = рекурсивному */
int dfs_csr_recursive(const graph_csr *g, int src, int *order);
int bfs_matrix(const graph_matrix *g, int src, int *order);
int dfs_matrix(const graph_matrix *g, int src, int *order);

int connected_components(const graph_csr *g, int *comp);          /* для неорієнтованого графа */
/* Топологічне сортування (Кан). Повертає V або -1, якщо є цикл. */
int topo_sort_kahn(const graph_csr *g, int *order);

#endif
