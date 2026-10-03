/* Why does linear growth via realloc() NOT produce quadratic time on the stack_growth chart?
 * Check: whether the block address changes after realloc, and how much it costs compared with
 * the "textbook" malloc(new) + memcpy. A blocker is allocated between the block and the realloc.
 * Result on macOS (measured): large blocks are extended IN PLACE (same address) in microseconds —
 * the allocator places large blocks in separate virtual-memory regions and simply grows the region.
 * Conclusion: the asymptotics "Θ(n²) copies with +K growth" is a property of the model, not a law of nature;
 * the real cost depends on the allocator. Doubling (×2) is the right strategy in any case. */
#include "dsa_common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    printf("%8s  %-10s %14s %26s\n", "size", "realloc", "realloc time", "malloc+memcpy of same size");
    for (size_t mb = 1; mb <= 256; mb *= 4) {
        size_t sz = mb << 20;
        char *p = malloc(sz);
        void *blocker = malloc(64);
        if (!p || !blocker) return 1;
        memset(p, 1, sz);
        uintptr_t old = (uintptr_t)p; /* compare as a number: using p after realloc is UB */
        uint64_t t0 = dsa_now_ns();
        char *q = realloc(p, sz + ((size_t)1 << 20));
        uint64_t t1 = dsa_now_ns();
        if (!q) return 1;
        char *r = malloc(sz);
        if (!r) return 1;
        uint64_t t2 = dsa_now_ns();
        memcpy(r, q, sz);
        uint64_t t3 = dsa_now_ns();
        volatile char sink = r[sz / 2];
        (void)sink;
        printf("%5zu MB  %-10s %11.1f us %23.1f us\n", mb, (uintptr_t)q == old ? "in place" : "MOVED",
               (double)(t1 - t0) / 1e3, (double)(t3 - t2) / 1e3);
        free(q); free(r); free(blocker);
    }
    puts("(memcpy includes the first touch of the new block's pages — page faults; that is the real cost of a copy)");
    return 0;
}
