# Експерименти

`make experiments` збирає й запускає всі. Кожен файл починається з коментаря: що саме в конспекті
неправильно, з номером сторінки, і як це виправити.

| Файл | Що відтворює | Збірка |
|---|---|---|
| `ex_ub_malloc_sizeof_pointer.c` | `malloc(sizeof(struct node *))` (notebook p.34) → heap-buffer-overflow | ASan+UBSan |
| `ex_ub_stack_isfull.c` | `isfull: top == MAXSIZE` (p.107) → global-buffer-overflow | ASan+UBSan |
| `ex_ub_counting_bucket.c` | `int bucket[max]` з індексами 0..max (p.83) → stack-buffer-overflow | ASan+UBSan |
| `ex_heapify_bug.c` | `largest != 1` (p.87) → нескінченна рекурсія (глибину обмежено лічильником) | -O0 |
| `ex_bubble_variants.c` | псевдокод з 1 проходу; «bubble», який насправді exchange sort; best case | -O0 |
| `ex_insertion_le.c` | `temp <= a[j]` → нестабільність і Θ(n²) на однакових ключах | -O0 |
| `ex_dfs_mark_on_push.c` | DFS з позначенням при push дає не-DFS порядок | -O0 |
| `ex_binary_overflow.c` | `(beg + end) / 2` → від'ємний mid | -O0 |
| `ex_struct_padding.c` | padding, `int mobile` для 10-значного номера, AoS vs SoA | -O2 |
| `ex_realloc_inplace.c` | чому через `realloc` не видно Θ(n²) лінійного росту | -O2 |
| `ex_small_n_crossover.c` | до якого n Θ(n²) insertion швидший за Θ(n log n) | -O2 |

`ex_ub_*` запускають баговий код у дочірньому процесі (`fork`) і друкують лише діагностичні рядки
санітайзера. Через це `make experiments` завершується успішно, хоча дочірні процеси падають — так задумано.
