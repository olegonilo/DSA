/* bench_ds — експерименти зі структурами даних (реальні вимірювання на цій машині):
 *  A. масив vs зв'язний список: обхід (послідовні вузли vs перемішані в пам'яті)  -> results/traverse.csv
 *  B. динамічний масив (стек): ріст ×2 vs +K                                      -> results/stack_growth.csv
 *  C. BST без балансування vs AVL: висота, середня глибина, час                   -> results/bst_avl.csv
 *  D. skip list: вплив p на кроки пошуку і пам'ять                                -> results/skiplist.csv
 *  E. побудова купи: Флойд O(n) vs n×push O(n log n)                               -> results/heap_build.csv
 */
#include "bench_util.h"
#include "dsa_list.h"
#include "dsa_skiplist.h"
#include "dsa_stack_queue.h"
#include "dsa_tree.h"

#include <math.h>

/* ------------------------------------------------------------------ A */
static void bench_traverse(void) {
    FILE *f = open_csv("results/traverse.csv", "layout,n,bytes_footprint,ns_per_elem");
    for (int lg = 8; lg <= 24; lg++) {
        size_t n = (size_t)1 << lg;
        int *arr = xmalloc(n * sizeof *arr);
        for (size_t i = 0; i < n; i++) arr[i] = (int)i;
        /* пул вузлів одним блоком — щоб контролювати розташування */
        sll_node *pool = xmalloc(n * sizeof *pool);
        size_t *perm = xmalloc(n * sizeof *perm);
        for (size_t i = 0; i < n; i++) perm[i] = i;
        int reps = lg <= 16 ? 9 : 5;
        for (int layout = 0; layout < 3; layout++) {
            sll_node *head = NULL;
            if (layout >= 1) {
                if (layout == 2) { /* Фішер–Єйтс: вузли зв'язані у випадковому порядку адрес */
                    dsa_rng r;
                    dsa_rng_seed(&r, 3);
                    for (size_t i = n - 1; i > 0; i--) {
                        size_t j = (size_t)dsa_rng_below(&r, i + 1);
                        size_t t = perm[i]; perm[i] = perm[j]; perm[j] = t;
                    }
                }
                for (size_t i = 0; i < n; i++) {
                    pool[perm[i]].data = (int)i;
                    pool[perm[i]].next = i + 1 < n ? &pool[perm[i + 1]] : NULL;
                }
                head = &pool[perm[0]];
            }
            /* Внутрішній прохід повторюється, доки один вимір не триватиме >= 100 мкс: при тіку
             * таймера 41.7 нс прохід по 256 int (~40 нс) інакше вимірюється одним тіком (знайдено на рев'ю). */
            size_t inner = 1;
            for (;;) {
                uint64_t t0 = dsa_now_ns();
                long long s = 0;
                for (size_t rep = 0; rep < inner; rep++) {
                    if (layout == 0) for (size_t i = 0; i < n; i++) s += arr[i];
                    else for (sll_node *p = head; p; p = p->next) s += p->data;
                    __asm__ volatile("" ::: "memory");
                }
                bench_sink += s;
                if (dsa_now_ns() - t0 >= 100000 || inner >= ((size_t)1 << 20)) break;
                inner *= 2;
            }
            uint64_t t[9];
            for (int k = 0; k < reps; k++) {
                long long s = 0;
                uint64_t t0 = dsa_now_ns();
                for (size_t rep = 0; rep < inner; rep++) {
                    if (layout == 0) for (size_t i = 0; i < n; i++) s += arr[i];
                    else for (sll_node *p = head; p; p = p->next) s += p->data;
                    /* бар'єр компілятора: пам'ять "могла змінитись", тож суму не можна винести з циклу
                     * повторів, а сам внутрішній цикл лишається ідентичним одиночному проходу */
                    __asm__ volatile("" ::: "memory");
                }
                t[k] = dsa_now_ns() - t0;
                bench_sink += s;
            }
            static const char *names[] = {"array", "list_sequential", "list_shuffled"};
            /* реальний обсяг: int — 4 Б, вузол sll_node — 16 Б */
            size_t footprint = n * (layout == 0 ? sizeof(int) : sizeof(sll_node));
            fprintf(f, "%s,%zu,%zu,%.4f\n", names[layout], n, footprint,
                    (double)median_u64(t, (size_t)reps) / ((double)n * (double)inner));
        }
        free(arr); free(pool); free(perm);
    }
    fclose(f);
}

/* ------------------------------------------------------------------ B */
/* Ріст "як у підручнику": новий блок + memcpy + free. realloc на macOS для великих блоків
 * переносить сторінки віртуальної пам'яті без копіювання байтів, тож квадратичне копіювання
 * лінійної стратегії через realloc НЕ видно. Цей варіант показує те, що описує теорія. */
typedef struct { int *d; size_t n, cap; size_t copied; } vec_copy;

static int vec_push_copy(vec_copy *v, int x, size_t k) {
    if (v->n == v->cap) {
        size_t nc = k ? v->cap + k : (v->cap ? v->cap * 2 : 8);
        int *nd = malloc(nc * sizeof *nd);
        if (!nd) return -1;
        if (v->n) memcpy(nd, v->d, v->n * sizeof *nd);
        v->copied += v->n;
        free(v->d);
        v->d = nd;
        v->cap = nc;
    }
    v->d[v->n++] = x;
    return 0;
}

static void bench_growth_copy(FILE *f) {
    for (int lg = 10; lg <= 22; lg++) {
        size_t n = (size_t)1 << lg;
        static const char *names[] = {"memcpy_double_x2", "memcpy_linear_+1024"};
        static const size_t ks[] = {0, 1024};
        for (int pol = 0; pol < 2; pol++) {
            if (pol == 1 && lg > 20) continue;
            uint64_t t[3];
            size_t copied = 0;
            for (int rep = 0; rep < 3; rep++) {
                vec_copy v = {0};
                uint64_t t0 = dsa_now_ns();
                for (size_t i = 0; i < n; i++) if (vec_push_copy(&v, (int)i, ks[pol])) exit(1);
                t[rep] = dsa_now_ns() - t0;
                copied = v.copied;
                bench_sink += v.d[n / 2];
                free(v.d);
            }
            uint64_t med = median_u64(t, 3);
            fprintf(f, "%s,%zu,%zu,%llu,%.3f\n", names[pol], n, copied, (unsigned long long)med, (double)med / (double)n);
        }
    }
}

static void bench_growth(void) {
    FILE *f = open_csv("results/stack_growth.csv", "policy,n,reallocs_or_copied,ns_total,ns_per_push");
    bench_growth_copy(f);
    for (int lg = 10; lg <= 22; lg++) {
        size_t n = (size_t)1 << lg;
        for (int pol = 0; pol < 3; pol++) {
            static const char *names[] = {"double_x2", "linear_+1024", "linear_+64"};
            static const size_t ks[] = {0, 1024, 64};
            if ((pol == 2 && lg > 17) || (pol == 1 && lg > 20)) continue; /* Θ(n^2/K) копіювань — обмежуємо час */
            uint64_t t[3];
            size_t reallocs = 0;
            for (int rep = 0; rep < 3; rep++) {
                stack s;
                stack_init(&s);
                /* Між realloc-ами алокуємо "заважаючий" блок, як у реальній програмі. Обмеження (рев'ю):
                 * блокерів не більше 64 і malloc блокера потрапляє у виміряний час; на результат це не
                 * впливає — ex_realloc_inplace показує, що великі блоки дорощуються на місці навіть з блокером. */
                void *noise[64];
                size_t nn = 0;
                uint64_t t0 = dsa_now_ns();
                for (size_t i = 0; i < n; i++) {
                    size_t before = s.reallocs;
                    int rc = pol == 0 ? stack_push(&s, (int)i) : stack_push_grow_linear(&s, (int)i, ks[pol]);
                    if (rc) { fprintf(stderr, "oom\n"); exit(1); }
                    if (s.reallocs != before && nn < 64) noise[nn++] = malloc(32);
                }
                t[rep] = dsa_now_ns() - t0;
                reallocs = s.reallocs;
                bench_sink += s.data[n / 2];
                stack_free(&s);
                for (size_t i = 0; i < nn; i++) free(noise[i]);
            }
            uint64_t med = median_u64(t, 3);
            fprintf(f, "%s,%zu,%zu,%llu,%.3f\n", names[pol], n, reallocs, (unsigned long long)med,
                    (double)med / (double)n);
        }
        fflush(f);
    }
    fclose(f);
}

/* ------------------------------------------------------------------ C */
static void depth_sum(const tnode *n, int d, double *sum) {
    if (!n) return;
    *sum += d;
    depth_sum(n->left, d + 1, sum);
    depth_sum(n->right, d + 1, sum);
}

static void bench_bst_avl(void) {
    FILE *f = open_csv("results/bst_avl.csv",
                       "tree,input,n,height,avg_depth,ns_per_insert,ns_per_search,rotations_per_insert");
    for (int lg = 6; lg <= 20; lg++) {
        size_t n = (size_t)1 << lg;
        int *keys = xmalloc(n * sizeof *keys);
        for (int input = 0; input < 2; input++) {
            for (size_t i = 0; i < n; i++) keys[i] = (int)i;
            if (input == 0) {
                dsa_rng r;
                dsa_rng_seed(&r, 11);
                for (size_t i = n - 1; i > 0; i--) {
                    size_t j = (size_t)dsa_rng_below(&r, i + 1);
                    int t = keys[i]; keys[i] = keys[j]; keys[j] = t;
                }
            }
            for (int tree = 0; tree < 2; tree++) {
                if (tree == 0 && input == 1 && lg > 15) continue; /* вироджений BST: Θ(n^2) вставка */
                tnode *root = NULL;
                g_tree.rotations = 0;
                uint64_t t0 = dsa_now_ns();
                for (size_t i = 0; i < n; i++)
                    root = tree == 0 ? bst_insert(root, keys[i], NULL) : avl_insert(root, keys[i]);
                uint64_t ti = dsa_now_ns() - t0;
                uint64_t rot = g_tree.rotations;
                t0 = dsa_now_ns();
                long long found = 0;
                size_t qs = n < 100000 ? n : 100000;
                for (size_t i = 0; i < qs; i++) found += bst_search(root, keys[(i * 7919) % n]) != NULL;
                uint64_t ts = dsa_now_ns() - t0;
                bench_sink += found;
                double ds = 0;
                depth_sum(root, 0, &ds);
                fprintf(f, "%s,%s,%zu,%d,%.4f,%.2f,%.2f,%.4f\n", tree ? "avl" : "bst", input ? "sorted" : "random",
                        n, tree_height(root), ds / (double)n, (double)ti / (double)n, (double)ts / (double)qs,
                        (double)rot / (double)n);
                tree_free(root);
            }
        }
        free(keys);
        fflush(f);
    }
    fclose(f);
}

/* ------------------------------------------------------------------ D */
static void bench_skiplist(void) {
    FILE *f = open_csv("results/skiplist.csv", "p,n,steps_per_search,pointers_per_node,ns_per_search,theory_steps");
    const double ps[] = {0.5, 0.25, 1.0 / 2.718281828459045, 0.125};
    for (size_t pi = 0; pi < 4; pi++) {
        for (int lg = 8; lg <= 20; lg += 2) {
            size_t n = (size_t)1 << lg;
            skiplist s;
            if (skip_init(&s, ps[pi], 2024)) exit(1);
            int *keys = xmalloc(n * sizeof *keys);
            dsa_rng r;
            dsa_rng_seed(&r, 5);
            for (size_t i = 0; i < n; i++) keys[i] = (int)(dsa_rng_next(&r) >> 34);
            for (size_t i = 0; i < n; i++) skip_insert(&s, keys[i]);
            const size_t Qn = 200000;
            s.steps = 0;
            uint64_t t0 = dsa_now_ns();
            long long hit = 0;
            for (size_t q = 0; q < Qn; q++) hit += skip_contains(&s, keys[dsa_rng_below(&r, n)]);
            uint64_t t = dsa_now_ns() - t0;
            bench_sink += hit;
            /* Pugh 1990: очікувана вартість пошуку <= L(n)/p + 1/(1-p) + 1, L(n) = log_{1/p} n.
             * Наш лічильник рахує і горизонтальні кроки, і кроки вниз — те саме, що й оцінка Пью. */
            double L = log((double)s.size) / log(1.0 / ps[pi]);
            double theory = L / ps[pi] + 1.0 / (1.0 - ps[pi]) + 1.0;
            fprintf(f, "%.4f,%zu,%.3f,%.4f,%.2f,%.3f\n", ps[pi], s.size, (double)s.steps / (double)Qn,
                    (double)s.pointers / (double)s.size, (double)t / (double)Qn, theory);
            skip_free(&s);
            free(keys);
        }
    }
    fclose(f);
}

/* ------------------------------------------------------------------ E */
static void bench_heap(void) {
    FILE *f = open_csv("results/heap_build.csv", "method,n,ns_per_elem");
    for (int lg = 10; lg <= 24; lg++) {
        size_t n = (size_t)1 << lg;
        int *src = xmalloc(n * sizeof *src);
        dsa_rng r;
        dsa_rng_seed(&r, 9);
        /* спадний вхід — worst case для push (кожен елемент спливає до кореня) */
        for (int input = 0; input < 2; input++) {
            if (input == 0) for (size_t i = 0; i < n; i++) src[i] = (int)(dsa_rng_next(&r) >> 33);
            else for (size_t i = 0; i < n; i++) src[i] = (int)(n - i);
            for (int m = 0; m < 2; m++) {
                uint64_t t[3];
                for (int rep = 0; rep < 3; rep++) {
                    minheap h;
                    heap_init(&h, n);
                    uint64_t t0 = dsa_now_ns();
                    if (m == 0) heap_build(&h, src, n);
                    else for (size_t i = 0; i < n; i++) heap_push(&h, src[i]);
                    t[rep] = dsa_now_ns() - t0;
                    if (!heap_is_valid(&h)) { fprintf(stderr, "heap BUG\n"); exit(1); }
                    heap_free(&h);
                }
                fprintf(f, "%s_%s,%zu,%.3f\n", m ? "push_n_times" : "floyd_build", input ? "descending" : "random",
                        n, (double)median_u64(t, 3) / (double)n);
            }
        }
        free(src);
    }
    fclose(f);
}

int main(int argc, char **argv) {
    /* без аргументів — усі секції; інакше лише названі: traverse growth bst skiplist heap */
    const char *only = argc > 1 ? argv[1] : NULL;
#define RUN_IF(name, fn) if (!only || strcmp(only, name) == 0) { fn(); puts("  " name " done"); }
    RUN_IF("traverse", bench_traverse)
    RUN_IF("growth", bench_growth)
    RUN_IF("bst", bench_bst_avl)
    RUN_IF("skiplist", bench_skiplist)
    RUN_IF("heap", bench_heap)
#undef RUN_IF
    return 0;
}
