/*
 * dsa_graph.h — graphs (sections 14-15 of the notes): representations and BFS/DFS traversals.
 *
 * Two representations, to compare them experimentally:
 *   - adjacency matrix: Θ(V^2) memory, O(1) edge check, Θ(V^2) traversal;
 *   - adjacency list in CSR format (compressed sparse row): Θ(V+E) memory, Θ(V+E) traversal.
 * Vertices are numbered 0..V-1. Neighbors are visited in ascending order,
 * so the traversal order is deterministic and matches a manual trace.
 */
#ifndef DSA_GRAPH_H
#define DSA_GRAPH_H

#include <stddef.h>
#include <stdint.h>

typedef struct { int u, v; } edge;

typedef struct {
    int V;
    unsigned char *m;   /* V*V, m[u*V+v] = 1 if there is an edge u->v */
} graph_matrix;

typedef struct {
    int V;
    size_t E;           /* number of directed arcs (undirected edge = 2 arcs) */
    size_t *off;        /* V+1 offsets */
    int *adj;           /* E neighbors */
} graph_csr;

int  gm_build(graph_matrix *g, int V, const edge *es, size_t ne, int undirected);
void gm_free(graph_matrix *g);
int  gc_build(graph_csr *g, int V, const edge *es, size_t ne, int undirected);
void gc_free(graph_csr *g);

/* Traversals return the number of visited vertices; order[] - the visit order,
 * dist[] (BFS, may be NULL) - number of edges from src, -1 if unreachable. */
int bfs_csr(const graph_csr *g, int src, int *order, int *dist);
int dfs_csr_iterative(const graph_csr *g, int src, int *order);   /* explicit stack, order = recursive */
int dfs_csr_recursive(const graph_csr *g, int src, int *order);
int bfs_matrix(const graph_matrix *g, int src, int *order);
int dfs_matrix(const graph_matrix *g, int src, int *order);

int connected_components(const graph_csr *g, int *comp);          /* for an undirected graph */
/* Topological sort (Kahn). Returns V, or -1 if there is a cycle. */
int topo_sort_kahn(const graph_csr *g, int *order);

#endif
