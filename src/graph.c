#include "dsa_graph.h"

#include <stdlib.h>
#include <string.h>

/* ================================================================ build */

int gm_build(graph_matrix *g, int V, const edge *es, size_t ne, int undirected) {
    g->V = V;
    g->m = calloc((size_t)V * (size_t)V, 1);
    if (!g->m) return -1;
    for (size_t i = 0; i < ne; i++) {
        g->m[(size_t)es[i].u * V + es[i].v] = 1;
        if (undirected) g->m[(size_t)es[i].v * V + es[i].u] = 1;
    }
    return 0;
}
void gm_free(graph_matrix *g) { free(g->m); g->m = NULL; }

static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

int gc_build(graph_csr *g, int V, const edge *es, size_t ne, int undirected) {
    size_t E = undirected ? 2 * ne : ne;
    g->V = V;
    g->E = E;
    g->off = calloc((size_t)V + 1, sizeof *g->off);
    g->adj = malloc((E ? E : 1) * sizeof *g->adj);
    if (!g->off || !g->adj) { gc_free(g); return -1; }
    for (size_t i = 0; i < ne; i++) {
        g->off[es[i].u + 1]++;
        if (undirected) g->off[es[i].v + 1]++;
    }
    for (int v = 0; v < V; v++) g->off[v + 1] += g->off[v];
    size_t *pos = malloc((size_t)V * sizeof *pos);
    if (!pos) { gc_free(g); return -1; }
    memcpy(pos, g->off, (size_t)V * sizeof *pos);
    for (size_t i = 0; i < ne; i++) {
        g->adj[pos[es[i].u]++] = es[i].v;
        if (undirected) g->adj[pos[es[i].v]++] = es[i].u;
    }
    free(pos);
    for (int v = 0; v < V; v++)
        qsort(g->adj + g->off[v], g->off[v + 1] - g->off[v], sizeof *g->adj, cmp_int);
    return 0;
}
void gc_free(graph_csr *g) { free(g->off); free(g->adj); g->off = NULL; g->adj = NULL; }

/* ================================================================ BFS / DFS on CSR */

int bfs_csr(const graph_csr *g, int src, int *order, int *dist) {
    int V = g->V;
    int *q = malloc((size_t)V * sizeof *q);
    int *d = dist ? dist : malloc((size_t)V * sizeof *d);
    if (!q || !d) { free(q); if (!dist) free(d); return -1; }
    for (int i = 0; i < V; i++) d[i] = -1;
    int head = 0, tail = 0, k = 0;
    /* mark as "visited" WHEN ENQUEUED, not when dequeued -
     * otherwise a vertex can enter the queue several times (up to E times). */
    d[src] = 0;
    q[tail++] = src;
    while (head < tail) {
        int u = q[head++];
        order[k++] = u;
        for (size_t e = g->off[u]; e < g->off[u + 1]; e++) {
            int v = g->adj[e];
            if (d[v] < 0) { d[v] = d[u] + 1; q[tail++] = v; }
        }
    }
    free(q);
    if (!dist) free(d);
    return k;
}

int dfs_csr_iterative(const graph_csr *g, int src, int *order) {
    /* Stack of frames (vertex, next edge index) - mimics recursion exactly,
     * so the visit order is the same as in recursive DFS. */
    int V = g->V;
    int *st = malloc((size_t)V * sizeof *st);
    size_t *it = malloc((size_t)V * sizeof *it);
    unsigned char *seen = calloc((size_t)V, 1);
    if (!st || !it || !seen) { free(st); free(it); free(seen); return -1; }
    int top = 0, k = 0;
    seen[src] = 1; order[k++] = src;
    st[top] = src; it[top] = g->off[src]; top++;
    while (top) {
        int u = st[top - 1];
        if (it[top - 1] == g->off[u + 1]) { top--; continue; }
        int v = g->adj[it[top - 1]++];
        if (!seen[v]) {
            seen[v] = 1; order[k++] = v;
            st[top] = v; it[top] = g->off[v]; top++;
        }
    }
    free(st); free(it); free(seen);
    return k;
}

static void dfs_rec(const graph_csr *g, int u, unsigned char *seen, int *order, int *k) {
    seen[u] = 1;
    order[(*k)++] = u;
    for (size_t e = g->off[u]; e < g->off[u + 1]; e++)
        if (!seen[g->adj[e]]) dfs_rec(g, g->adj[e], seen, order, k);
}

int dfs_csr_recursive(const graph_csr *g, int src, int *order) {
    unsigned char *seen = calloc((size_t)g->V, 1);
    if (!seen) return -1;
    int k = 0;
    dfs_rec(g, src, seen, order, &k);
    free(seen);
    return k;
}

/* ================================================================ BFS / DFS on matrix */

int bfs_matrix(const graph_matrix *g, int src, int *order) {
    int V = g->V;
    int *q = malloc((size_t)V * sizeof *q);
    unsigned char *seen = calloc((size_t)V, 1);
    if (!q || !seen) { free(q); free(seen); return -1; }
    int head = 0, tail = 0, k = 0;
    seen[src] = 1; q[tail++] = src;
    while (head < tail) {
        int u = q[head++];
        order[k++] = u;
        const unsigned char *row = g->m + (size_t)u * V;
        for (int v = 0; v < V; v++)               /* Θ(V) per vertex => Θ(V^2) total */
            if (row[v] && !seen[v]) { seen[v] = 1; q[tail++] = v; }
    }
    free(q); free(seen);
    return k;
}

int dfs_matrix(const graph_matrix *g, int src, int *order) {
    int V = g->V;
    int *st = malloc((size_t)V * sizeof *st), *it = malloc((size_t)V * sizeof *it);
    unsigned char *seen = calloc((size_t)V, 1);
    if (!st || !it || !seen) { free(st); free(it); free(seen); return -1; }
    int top = 0, k = 0;
    seen[src] = 1; order[k++] = src;
    st[top] = src; it[top] = 0; top++;
    while (top) {
        int u = st[top - 1];
        const unsigned char *row = g->m + (size_t)u * V;
        int v = it[top - 1];
        while (v < V && !(row[v] && !seen[v])) v++;
        if (v == V) { top--; continue; }
        it[top - 1] = v + 1;
        seen[v] = 1; order[k++] = v;
        st[top] = v; it[top] = 0; top++;
    }
    free(st); free(it); free(seen);
    return k;
}

/* ================================================================ applications */

int connected_components(const graph_csr *g, int *comp) {
    int V = g->V, c = 0;
    int *q = malloc((size_t)V * sizeof *q);
    if (!q) return -1;
    for (int i = 0; i < V; i++) comp[i] = -1;
    for (int s = 0; s < V; s++) {
        if (comp[s] >= 0) continue;
        int head = 0, tail = 0;
        comp[s] = c; q[tail++] = s;
        while (head < tail) {
            int u = q[head++];
            for (size_t e = g->off[u]; e < g->off[u + 1]; e++)
                if (comp[g->adj[e]] < 0) { comp[g->adj[e]] = c; q[tail++] = g->adj[e]; }
        }
        c++;
    }
    free(q);
    return c;
}

int topo_sort_kahn(const graph_csr *g, int *order) {
    int V = g->V;
    int *indeg = calloc((size_t)V, sizeof *indeg), *q = malloc((size_t)V * sizeof *q);
    if (!indeg || !q) { free(indeg); free(q); return -1; }
    for (size_t e = 0; e < g->E; e++) indeg[g->adj[e]]++;
    int head = 0, tail = 0, k = 0;
    for (int v = 0; v < V; v++) if (!indeg[v]) q[tail++] = v;
    while (head < tail) {
        int u = q[head++];
        order[k++] = u;
        for (size_t e = g->off[u]; e < g->off[u + 1]; e++)
            if (--indeg[g->adj[e]] == 0) q[tail++] = g->adj[e];
    }
    free(indeg); free(q);
    return k == V ? V : -1;
}
