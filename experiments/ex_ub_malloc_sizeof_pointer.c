/* Конспект, notebook p.34 (PDF p.35), вставка у список:
 *     ptr = (struct node *) malloc(sizeof(struct node *)      <- так у конспекті
 * sizeof(struct node *) — розмір ВКАЗІВНИКА (8 байт на 64-біт), а не вузла.
 * Для struct node { int data; struct node *next; } на LP64 sizeof = 16 (4 + 4 padding + 8).
 * Запис ptr->next = ... виходить за межі виділеного блоку — heap-buffer-overflow (UB, C11 J.2).
 * Виправлення: ptr = malloc(sizeof *ptr);  — тип береться з самої змінної, помилитись неможливо. */
#include "exp_util.h"

#include <stddef.h>

struct node {
    int data;
    struct node *next;
};

static void notes_version(void) {
    struct node *ptr = (struct node *)malloc(sizeof(struct node *));
    ptr->data = 10;
    ptr->next = NULL; /* байти 8..15 — поза блоком з 8 байт */
    free(ptr);
}

static void fixed_version(void) {
    struct node *ptr = malloc(sizeof *ptr);
    if (!ptr) return;
    ptr->data = 10;
    ptr->next = NULL;
    free(ptr);
}

int main(void) {
    printf("sizeof(struct node *) = %zu, sizeof(struct node) = %zu, offsetof(next) = %zu\n",
           sizeof(struct node *), sizeof(struct node), offsetof(struct node, next));
    puts("1) код з конспекту: malloc(sizeof(struct node *))");
    run_in_child("конспект", notes_version);
    puts("2) виправлено: malloc(sizeof *ptr)");
    run_in_child("виправлено", fixed_version);
    return 0;
}
