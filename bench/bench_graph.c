/* bench_graph — BFS на матриці суміжності vs списку суміжності (CSR).
 *  1) розріджений граф (середній степінь 8), V росте: матриця Θ(V^2), CSR Θ(V+E);
 *  2) фіксоване V = 4096, щільність росте: точка, де матриця наздоганяє CSR.
 * Вихід: results/graph_bfs.csv, results/graph_density.csv */
#include "bench_util.h"
#include "dsa_graph.h"

/* Простий неорієнтований граф: ланцюжок 0-1-...-(V-1) (зв'язність) + випадкові РІЗНІ ребра без петель.
 * Перша версія допускала дублікати й петлі: CSR зберігав дублікати, а матриця — ні, тож CSR
 * мав до 1.58× більше дуг, ніж матриця (знайдено на рев'ю). Тепер дублікати відкидаються через бітову матрицю. */
static edge *random_edges(int V, size_t *ne_inout, dsa_rng *r) {
    size_t maxe = (size_t)V * (size_t)(V - 1) / 2, ne = *ne_inout < maxe ? *ne_inout : maxe;
    edge *es = xmalloc((ne ? ne : 1) * sizeof *es);
    unsigned char *seen = calloc((size_t)V * (size_t)V, 1);
    if (!seen) { fprintf(stderr, "oom\n"); exit(1); }
    size_t k = 0;
    for (int v = 0; v + 1 < V && k < ne; v++) {
        es[k].u = v; es[k].v = v + 1; k++;
        seen[(size_t)v * V + v + 1] = seen[(size_t)(v + 1) * V + v] = 1;
    }
    if (ne * 4 > maxe * 3) {  /* дуже щільно: перебираємо всі пари і беремо кожну з імовірністю */
        for (int u = 0; u < V && k < ne; u++)
            for (int v = u + 1; v < V && k < ne; v++)
                if (!seen[(size_t)u * V + v] && dsa_rng_below(r, maxe) < ne) { es[k].u = u; es[k].v = v; k++; }
    } else {
        while (k < ne) {
            int u = (int)dsa_rng_below(r, (uint64_t)V), v = (int)dsa_rng_below(r, (uint64_t)V);
            if (u == v || seen[(size_t)u * V + v]) continue;
            seen[(size_t)u * V + v] = seen[(size_t)v * V + u] = 1;
            es[k].u = u; es[k].v = v; k++;
        }
    }
    free(seen);
    *ne_inout = k;
    return es;
}

static void measure(FILE *f, const char *tag, int V, size_t ne, dsa_rng *r) {
    edge *es = random_edges(V, &ne, r);
    graph_csr g;
    graph_matrix m;
    if (gc_build(&g, V, es, ne, 1) || gm_build(&m, V, es, ne, 1)) { fprintf(stderr, "oom\n"); exit(1); }
    int *order = xmalloc((size_t)V * sizeof *order);
    uint64_t tc[5], tm[5];
    for (int k = 0; k < 5; k++) {
        uint64_t t0 = dsa_now_ns();
        int a = bfs_csr(&g, 0, order, NULL);
        tc[k] = dsa_now_ns() - t0;
        t0 = dsa_now_ns();
        int b = bfs_matrix(&m, 0, order);
        tm[k] = dsa_now_ns() - t0;
        if (a != V || b != V) { fprintf(stderr, "BFS BUG\n"); exit(1); }
    }
    size_t mem_csr = ((size_t)V + 1) * sizeof(size_t) + g.E * sizeof(int);
    size_t mem_mat = (size_t)V * (size_t)V;
    fprintf(f, "%s,%d,%zu,%llu,%llu,%zu,%zu\n", tag, V, g.E / 2, (unsigned long long)median_u64(tc, 5),
            (unsigned long long)median_u64(tm, 5), mem_csr, mem_mat);
    gc_free(&g);
    gm_free(&m);
    free(order);
    free(es);
}

int main(void) {
    dsa_rng r;
    dsa_rng_seed(&r, 77);
    FILE *f = open_csv("results/graph_bfs.csv", "graph,V,E,ns_csr,ns_matrix,bytes_csr,bytes_matrix");
    for (int lg = 6; lg <= 14; lg++) measure(f, "sparse_deg8", 1 << lg, (size_t)4 << lg, &r);
    fclose(f);
    f = open_csv("results/graph_density.csv", "graph,V,E,ns_csr,ns_matrix,bytes_csr,bytes_matrix");
    const int V = 4096;
    for (size_t deg = 2; deg <= 4096; deg *= 2) measure(f, "V4096", V, (size_t)V * deg / 2, &r); /* deg 4096 → повний граф */
    fclose(f);
    puts("  graph done");
    return 0;
}
