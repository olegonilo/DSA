/* The notes, DFS (notebook p.68–69, PDF p.69–70):
 *   step 4: Pop top node N. Process it, STATUS = 3.
 *   step 5: Push on stack all neighbours of N that are in ready state (STATUS = 1), set STATUS = 2.
 * A vertex is marked "waiting" AT PUSH TIME. So if a vertex is already on the stack and later
 * turns out to be a neighbour of a deeper vertex, it cannot be pushed again on top — and the processing
 * order stops being a depth-first order (there is no guarantee it is a DFS traversal of any DFS tree).
 *
 * Counterexample: edges A–B, A–C, A–D, B–D. Neighbours are pushed in alphabetical order.
 *   The notes: push A; pop A -> push B,C,D; pop D -> (B already waiting) nothing; pop C; pop B
 *              order: A D C B. After D a real DFS must go to B (a neighbour of D, not yet visited),
 *              but here C is processed — not a neighbour of D. So A D C B is not a DFS order.
 *   Correct:   mark on POP (a vertex may sit on the stack several times) or use recursion.
 *              Order with the same push rule: A D B C. */
#include <stdio.h>

enum { V = 4 };
static const char *NAME = "ABCD";
static const int adj[V][V] = {
    /* A  B  C  D */
    {0, 1, 1, 1}, /* A */
    {1, 0, 0, 1}, /* B */
    {1, 0, 0, 0}, /* C */
    {1, 1, 0, 0}, /* D */
};

static void dfs_notes(void) {
    int status[V], st[V * V], top = 0;
    for (int i = 0; i < V; i++) status[i] = 1;
    st[top++] = 0; status[0] = 2;
    printf("notes     (mark on push): ");
    while (top) {
        int n = st[--top];
        printf("%c ", NAME[n]);
        status[n] = 3;
        for (int v = 0; v < V; v++)
            if (adj[n][v] && status[v] == 1) { st[top++] = v; status[v] = 2; }
    }
    printf("\n");
}

static void dfs_mark_on_pop(void) {
    int seen[V] = {0}, st[V * V], top = 0;
    st[top++] = 0;
    printf("correct   (mark on pop):  ");
    while (top) {
        int n = st[--top];
        if (seen[n]) continue;
        seen[n] = 1;
        printf("%c ", NAME[n]);
        for (int v = 0; v < V; v++)
            if (adj[n][v] && !seen[v]) st[top++] = v;
    }
    printf("\n");
}

int main(void) {
    dfs_notes();
    dfs_mark_on_pop();
    return 0;
}
