/* The notes, notebook p.34 (PDF p.35), list insertion:
 *     ptr = (struct node *) malloc(sizeof(struct node *)      <- as written in the notes
 * sizeof(struct node *) is the size of a POINTER (8 bytes on 64-bit), not of a node.
 * For struct node { int data; struct node *next; } on LP64 sizeof = 16 (4 + 4 padding + 8).
 * Writing ptr->next = ... goes past the allocated block — heap-buffer-overflow (UB, C11 J.2).
 * Fix: ptr = malloc(sizeof *ptr);  — the type comes from the variable itself, so it cannot be wrong. */
#include "exp_util.h"

#include <stddef.h>

struct node {
    int data;
    struct node *next;
};

static void notes_version(void) {
    struct node *ptr = (struct node *)malloc(sizeof(struct node *));
    ptr->data = 10;
    ptr->next = NULL; /* bytes 8..15 are outside the 8-byte block */
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
    puts("1) code from the notes: malloc(sizeof(struct node *))");
    run_in_child("the notes", notes_version);
    puts("2) fixed: malloc(sizeof *ptr)");
    run_in_child("fixed", fixed_version);
    return 0;
}
