/* Конспект, DFS (notebook p.68–69, PDF p.69–70):
 *   step 4: Pop top node N. Process it, STATUS = 3.
 *   step 5: Push on stack all neighbours of N that are in ready state (STATUS = 1), set STATUS = 2.
 * Вершина позначається "waiting" В МОМЕНТ PUSH. Тому якщо вершина вже лежить у стеку, а пізніше
 * виявляється сусідом глибшої вершини, її не можна покласти ще раз зверху — і порядок обробки
 * перестає бути порядком пошуку в глибину (немає гарантії, що це DFS-обхід якогось DFS-дерева).
 *
 * Контрприклад: ребра A–B, A–C, A–D, B–D. Сусіди проштовхуються в алфавітному порядку.
 *   Конспект:  push A; pop A -> push B,C,D; pop D -> (B вже waiting) нічого; pop C; pop B
 *              порядок: A D C B. Після D справжній DFS мусить піти в B (сусід D, ще не відвіданий),
 *              а тут обробляється C — не сусід D. Отже A D C B не є DFS-порядком.
 *   Правильно: позначати при POP (вершина може лежати в стеку кілька разів) або рекурсія.
 *              Порядок з тим же правилом проштовхування: A D B C. */
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
    printf("конспект (mark on push): ");
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
    printf("правильно (mark on pop): ");
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
