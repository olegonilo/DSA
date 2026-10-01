#!/usr/bin/env python3
"""Будує всі графіки charts/*.png ВИКЛЮЧНО з results/*.csv та errata/errata.csv.

Жодне число на графіках не задане вручну: якщо CSV немає — графік пропускається з повідомленням.
Палітра — перевірений категорійний набір (CVD-safe для суміжних пар); ідентичність серії
дублюється маркером, типом лінії і підписом на кінці лінії (не тільки кольором).
"""
import csv
import math
import os
import sys
from collections import defaultdict

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt  # noqa: E402
from matplotlib.ticker import FuncFormatter  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
RES = os.path.join(ROOT, "results")
OUT = os.path.join(ROOT, "charts")
os.makedirs(OUT, exist_ok=True)

SERIES = ["#2a78d6", "#eb6834", "#1baf7a", "#eda100", "#e87ba4", "#008300", "#4a3aa7", "#e34948"]
MARKERS = ["o", "s", "^", "D", "v", "P", "X", "*"]
DASHES = ["-", "--", "-.", ":", "-", "--", "-.", ":"]
SURFACE = "#fcfcfb"
INK = "#0b0b0b"
INK2 = "#52514e"
GRID = "#e4e3df"
SEQ = ["#cde2fb", "#9ec5f4", "#6da7ec", "#3987e5", "#256abf", "#184f95", "#0d366b"]

plt.rcParams.update({
    "figure.facecolor": SURFACE, "axes.facecolor": SURFACE, "savefig.facecolor": SURFACE,
    "axes.edgecolor": GRID, "axes.labelcolor": INK2, "xtick.color": INK2, "ytick.color": INK2,
    "text.color": INK, "axes.grid": True, "grid.color": GRID, "grid.linewidth": 0.8,
    "axes.spines.top": False, "axes.spines.right": False, "font.size": 10,
    "axes.titlesize": 12, "axes.titleweight": "bold", "axes.titlelocation": "left",
    "legend.frameon": False, "lines.linewidth": 2, "lines.markersize": 5,
})

MACHINE = "Apple M4 Pro, macOS 27, Apple clang 21, -O2 -march=native"


def read(name):
    path = os.path.join(RES, name) if not name.startswith("/") else name
    if not os.path.exists(path):
        print(f"  skip: {path} not found", file=sys.stderr)
        return None
    with open(path, newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f))


def bytes_fmt(x, _pos=None):
    for unit, k in (("ГБ", 1 << 30), ("МБ", 1 << 20), ("КБ", 1 << 10)):
        if x >= k:
            v = x / k
            return f"{v:.0f} {unit}" if v >= 10 or v == int(v) else f"{v:.1f} {unit}"
    return f"{x:.0f} Б"


def n_fmt(x, _pos=None):
    if x >= 1 << 20:
        return f"{x / (1 << 20):.0f}M" if x % (1 << 20) == 0 else f"{x / (1 << 20):.1f}M"
    if x >= 1 << 10:
        return f"{x / (1 << 10):.0f}K"
    return f"{x:.0f}"


_PENDING = defaultdict(list)


def end_label(ax, xs, ys, text, color, dy=0):
    """Підпис на кінці лінії. Розміщення відкладається до finish(): там підписи однієї осі
    розсуваються по вертикалі, щоб не перекриватись."""
    if not xs:
        return
    _PENDING[ax].append((xs[-1], ys[-1], text, color))


def _place_labels(fig):
    fig.canvas.draw()
    for ax, items in list(_PENDING.items()):
        if ax.figure is not fig:
            continue
        pts = []
        for x, y, text, color in items:
            px, py = ax.transData.transform((x, y))
            pts.append([py, px, x, y, text, color])
        pts.sort(key=lambda t: t[0])
        gap = 13.0  # пікселів між підписами
        for i in range(1, len(pts)):
            if pts[i][0] - pts[i - 1][0] < gap:
                pts[i][0] = pts[i - 1][0] + gap
        for py_new, px, x, y, text, color in pts:
            _, py0 = ax.transData.transform((x, y))
            ax.annotate(text, (x, y), xytext=(8, (py_new - py0) / fig.dpi * 72), textcoords="offset points",
                        color=INK, fontsize=8.5, va="center",
                        bbox=dict(boxstyle="round,pad=0.15", fc=SURFACE, ec=color, lw=1),
                        arrowprops=dict(arrowstyle="-", color=color, lw=0.6) if abs(py_new - py0) > 3 else None)
        del _PENDING[ax]


ALGO_SLOT = {}


def slot_for(name):
    """Колір прив'язаний до сутності (алгоритму), а не до позиції в легенді."""
    if name not in ALGO_SLOT:
        ALGO_SLOT[name] = len(ALGO_SLOT) % 8
    return ALGO_SLOT[name]


def line(ax, xs, ys, i, label, direct=True, dy=0):
    ax.plot(xs, ys, color=SERIES[i % 8], marker=MARKERS[i % 8], linestyle=DASHES[i % 8], label=label,
            markeredgecolor=SURFACE, markeredgewidth=0.8)
    if direct:
        end_label(ax, xs, ys, label, SERIES[i % 8], dy)


def finish(fig, name, note=None):
    fig.tight_layout(rect=(0, 0.03, 1, 1))
    _place_labels(fig)
    fig.text(0.01, 0.005, note or MACHINE, fontsize=7.5, color=INK2, ha="left", va="bottom")
    fig.savefig(os.path.join(OUT, name), dpi=150, bbox_inches="tight", pad_inches=0.15)
    plt.close(fig)
    print("  ", name)


def cache_lines(ax, horizontal=False):
    for size, name in ((128 << 10, "L1d 128 КБ"), (16 << 20, "L2 16 МБ")):
        ax.axvline(size, color=INK2, lw=1, ls=(0, (2, 3)))
        ax.text(size, 1.0, " " + name, transform=ax.get_xaxis_transform(), fontsize=8, color=INK2,
                va="top", ha="left")


# ------------------------------------------------------------------ sorting

def plot_sort_time():
    rows = read("sort_time.csv")
    if not rows:
        return
    data = defaultdict(lambda: ([], []))
    for r in rows:
        if r["input"] == "random":
            d = data[r["algo"]]
            d[0].append(int(r["n"]))
            d[1].append(float(r["ns_per_elem"]))
    groups = [
        ("Θ(n²): час на елемент росте лінійно з n",
         ["bubble_naive", "bubble", "selection", "insertion", "insertion_binary"]),
        ("O(n log n) і Shell: час на елемент росте ~log n",
         ["shell_ciura", "merge_topdown", "quick_median3", "quick_3way", "heap", "libc_qsort"]),
        ("Не порівняльні (ключі в [0, n)) vs quick_median3",
         ["counting", "radix_lsd", "bucket", "quick_median3"]),
    ]
    fig, axes = plt.subplots(1, 3, figsize=(17, 5.4))
    for ax, (title, algos) in zip(axes, groups):
        for i, a in enumerate(algos):
            xs, ys = data[a]
            # quick_median3 — той самий колір на обох панелях; не порівняльні — свої слоти
            slot = {"quick_median3": 2, "counting": 6, "radix_lsd": 7, "bucket": 4}.get(a, i) if ax is axes[2] else i
            line(ax, xs, ys, slot, a)
        ax.set_xscale("log", base=2)
        ax.set_yscale("log")
        ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
        ax.set_title(title, fontsize=10.5)
        ax.set_xlabel("n (випадкові int)")
        ax.set_ylabel("нс на елемент (медіана)")
        ax.margins(x=0.25)
    fig.suptitle("Сортування: виміряний час / n", x=0.01, ha="left", fontweight="bold")
    finish(fig, "sort_time_random.png")


def plot_sort_heatmap():
    rows = read("sort_time.csv")
    if not rows:
        return
    N = 16384
    inputs = ["random", "sorted", "reversed", "nearly_sorted", "few_unique"]
    algos = []
    val = {}
    for r in rows:
        if int(r["n"]) == N:
            if r["algo"] not in algos:
                algos.append(r["algo"])
            val[(r["algo"], r["input"])] = float(r["ns_per_elem"])
    if not algos:
        return
    fig, ax = plt.subplots(figsize=(9.5, 7.5))
    vmax = max(math.log10(v) for v in val.values())
    vmin = min(math.log10(v) for v in val.values())
    from matplotlib.colors import LinearSegmentedColormap
    cmap = LinearSegmentedColormap.from_list("seq", SEQ)
    grid = [[math.log10(val.get((a, i), float("nan"))) if (a, i) in val else float("nan") for i in inputs]
            for a in algos]
    ax.imshow(grid, cmap=cmap, vmin=vmin, vmax=vmax, aspect="auto")
    for y, a in enumerate(algos):
        for x, i in enumerate(inputs):
            if (a, i) in val:
                v = val[(a, i)]
                lum_dark = (math.log10(v) - vmin) / (vmax - vmin) > 0.55
                ax.text(x, y, f"{v:.0f}" if v >= 10 else f"{v:.1f}", ha="center", va="center", fontsize=8.5,
                        color="#ffffff" if lum_dark else INK)
            else:
                ax.text(x, y, "—", ha="center", va="center", color=INK2)
    ax.set_xticks(range(len(inputs)), inputs)
    ax.set_yticks(range(len(algos)), algos)
    ax.grid(False)
    ax.set_title(f"Сортування, n = {N}: нс на елемент (темніше = повільніше, лог. шкала)")
    finish(fig, "sort_heatmap.png")


def plot_sort_ops():
    rows = read("sort_ops.csv")
    if not rows:
        return
    d = defaultdict(lambda: ([], []))
    for r in rows:
        n = int(r["n"])
        d[(r["algo"], r["input"])][0].append(n)
        d[(r["algo"], r["input"])][1].append(int(r["cmp"]))
    fig, axes = plt.subplots(1, 2, figsize=(15, 5.4))
    ax = axes[0]
    for i, a in enumerate(["merge_topdown", "heap", "quick_median3", "quick_hoare_mid", "shell_ciura", "libc_qsort"]):
        xs, cs = d[(a, "random")]
        ys = [c / (n * math.log2(n)) for n, c in zip(xs, cs)]
        line(ax, xs, ys, i, a)
    # Нижня межа для порівняльних сортувань: ceil(log2(n!)) порівнянь у гіршому випадку
    # (і log2(n!) - O(1) у середньому). log2(n!) = n·log2 n − 1.4427·n + O(log n) < n·log2 n.
    xs = d[("merge_topdown", "random")][0]
    lb = [math.lgamma(n + 1) / math.log(2) / (n * math.log2(n)) for n in xs]
    ax.plot(xs, lb, color=INK2, lw=1.2, ls=":")
    end_label(ax, xs, lb, "log₂(n!) — нижня межа", INK2)
    ax.set_xscale("log", base=2)
    ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
    ax.set_title("Порівняння / (n·log₂n), випадковий вхід")
    ax.set_xlabel("n")
    ax.set_ylabel("cmp / (n log₂ n)")
    ax.margins(x=0.25)
    ax = axes[1]
    combos = [("insertion", "random", "insertion · random → n²/4"),
              ("insertion", "reversed", "insertion · reversed → n²/2"),
              ("selection", "sorted", "selection · будь-який → n²/2"),
              ("bubble", "sorted", "bubble · sorted → n (best)"),
              ("quick_lomuto_last", "sorted", "lomuto(last) · sorted → n²/2"),
              ("quick_median3", "sorted", "median3 · sorted")]
    for i, (a, inp, lab) in enumerate(combos):
        xs, cs = d[(a, inp)]
        line(ax, xs, [c / (n * n) for n, c in zip(xs, cs)], i, lab)
    ax.set_xscale("log", base=2)
    ax.set_yscale("log")
    ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
    ax.set_ylim(2e-5, 4)
    ax.set_title("Порівняння / n²: константа при n² (точні лічильники)")
    ax.set_xlabel("n")
    ax.set_ylabel("cmp / n²")
    ax.margins(x=0.45)
    finish(fig, "sort_ops.png", "Точна кількість порівнянь (детерміновано, -DDSA_COUNT) — не залежить від машини")


# ------------------------------------------------------------------ searching

def plot_search_time():
    rows = read("search_time.csv")
    if not rows:
        return
    d = defaultdict(lambda: ([], []))
    for r in rows:
        d[r["algo"]][0].append(int(r["bytes"]))
        d[r["algo"]][1].append(float(r["ns_per_query"]))
    fig, ax = plt.subplots(figsize=(11, 6))
    for i, a in enumerate(["linear", "binary", "branchless", "eytzinger", "eytzinger_unclamped", "interpolation"]):
        xs, ys = d[a]
        line(ax, xs, ys, i, a)
    cache_lines(ax)
    ax.set_xscale("log", base=2)
    ax.set_yscale("log")
    ax.xaxis.set_major_formatter(FuncFormatter(bytes_fmt))
    ax.set_xlabel("розмір масиву (відсортовані рівномірно-випадкові int)")
    ax.set_ylabel("нс на один успішний пошук (медіана з 5 × 2²⁰ запитів)")
    ax.set_title("Пошук: та сама O(log n), різна взаємодія з кешем і передбачувачем гілок")
    ax.margins(x=0.2)
    finish(fig, "search_time.png")


def plot_search_ops():
    rows = read("search_ops.csv")
    if not rows:
        return
    d = defaultdict(lambda: ([], []))
    for r in rows:
        d[(r["algo"], r["data"])][0].append(int(r["n"]))
        d[(r["algo"], r["data"])][1].append(float(r["avg_cmp"]))
    fig, ax = plt.subplots(figsize=(11, 6))
    combos = [("binary", "uniform_random", "binary"),
              ("interpolation", "arithmetic", "interpolation · арифм. прогресія"),
              ("interpolation", "uniform_random", "interpolation · рівномірні випадкові"),
              ("interpolation", "skewed_x4", "interpolation · скошені (x⁴)"),
              ("jump", "uniform_random", "jump (√n)"), ("ternary", "uniform_random", "ternary")]
    for i, (a, dat, lab) in enumerate(combos):
        xs, ys = d[(a, dat)]
        line(ax, xs, ys, i, lab)
    ax.set_xscale("log", base=2)
    ax.set_yscale("log")
    ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
    ax.set_xlabel("n")
    ax.set_ylabel("середня кількість порівнянь ключів")
    ax.set_title("Пошук: кількість порівнянь (успішний пошук, 2000 випадкових ключів)")
    ax.margins(x=0.3)
    finish(fig, "search_ops.png", "Точні лічильники (-DDSA_COUNT). Ternary робить МЕНШЕ ітерацій, але БІЛЬШЕ порівнянь, ніж binary")


# ------------------------------------------------------------------ data structures

def plot_traverse():
    rows = read("traverse.csv")
    if not rows:
        return
    d = defaultdict(lambda: ([], []))
    for r in rows:  # bytes_footprint: int = 4 Б, вузол sll_node = 16 Б — реальний обсяг структури
        d[r["layout"]][0].append(int(r["bytes_footprint"]))
        d[r["layout"]][1].append(float(r["ns_per_elem"]))
    fig, ax = plt.subplots(figsize=(11, 6))
    labels = {"array": "масив int", "list_sequential": "список, вузли підряд у пам'яті",
              "list_shuffled": "список, вузли перемішані"}
    for i, k in enumerate(["array", "list_sequential", "list_shuffled"]):
        line(ax, d[k][0], d[k][1], i, labels[k])
    cache_lines(ax)
    ax.set_xscale("log", base=2)
    ax.set_yscale("log")
    ax.xaxis.set_major_formatter(FuncFormatter(bytes_fmt))
    ax.set_xlabel("обсяг пам'яті структури")
    ax.set_ylabel("нс на елемент обходу (сума всіх елементів)")
    ratio = d["list_shuffled"][1][-1] / d["array"][1][-1]
    ax.set_title(f"Обхід Θ(n) у всіх трьох випадках, але на великих n перемішаний список повільніший "
                 f"за масив у {ratio:.0f} разів")
    ax.margins(x=0.25)
    finish(fig, "traverse.png")


def plot_growth():
    rows = read("stack_growth.csv")
    if not rows:
        return
    d = defaultdict(lambda: ([], []))
    for r in rows:
        d[r["policy"]][0].append(int(r["n"]))
        d[r["policy"]][1].append(float(r["ns_per_push"]))
    fig, axes = plt.subplots(1, 2, figsize=(15, 5.4), sharey=True)
    for ax, (title, pols) in zip(axes, [
        ("Явне копіювання (malloc + memcpy + free) — як у теорії", ["memcpy_double_x2", "memcpy_linear_+1024"]),
        ("realloc() на macOS — великі блоки дорощуються на місці (див. ex_realloc_inplace)", ["double_x2", "linear_+1024", "linear_+64"]),
    ]):
        for i, p in enumerate(pols):
            line(ax, d[p][0], d[p][1], i, p)
        ax.set_xscale("log", base=2)
        ax.set_yscale("log")
        ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
        ax.set_title(title, fontsize=10.5)
        ax.set_xlabel("n push-ів")
        ax.margins(x=0.3)
    axes[0].set_ylabel("нс на один push (амортизовано)")
    finish(fig, "stack_growth.png")


def plot_bst_avl():
    rows = read("bst_avl.csv")
    if not rows:
        return
    d = defaultdict(lambda: ([], [], [], []))
    for r in rows:
        k = (r["tree"], r["input"])
        d[k][0].append(int(r["n"]))
        d[k][1].append(int(r["height"]))
        d[k][2].append(float(r["ns_per_search"]))
        d[k][3].append(float(r["avg_depth"]))
    fig, axes = plt.subplots(1, 2, figsize=(15, 5.6))
    ax = axes[0]
    labels = {("bst", "random"): "BST · випадковий порядок", ("bst", "sorted"): "BST · відсортований (вироджений)",
              ("avl", "random"): "AVL · випадковий", ("avl", "sorted"): "AVL · відсортований"}
    for i, k in enumerate(labels):
        line(ax, d[k][0], d[k][1], i, labels[k])
    ns = d[("avl", "random")][0]
    ax.plot(ns, [math.log2(n + 1) for n in ns], color=INK2, lw=1, ls=":")
    ax.plot(ns, [1.4405 * math.log2(n + 2) - 0.3277 for n in ns], color=INK2, lw=1, ls="--")
    end_label(ax, ns, [math.log2(n + 1) for n in ns], "log₂(n+1) — мінімум", INK2, -8)
    end_label(ax, ns, [1.4405 * math.log2(n + 2) - 0.3277 for n in ns], "1.44·log₂(n+2) — межа AVL", INK2, 8)
    ax.set_xscale("log", base=2)
    ax.set_yscale("log", base=2)
    ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
    ax.set_xlabel("n ключів")
    ax.set_ylabel("висота (кількість рівнів)")
    ax.set_title("Висота дерева")
    ax.margins(x=0.45)
    ax = axes[1]
    for i, k in enumerate(labels):
        line(ax, d[k][0], d[k][2], i, labels[k])
    ax.set_xscale("log", base=2)
    ax.set_yscale("log")
    ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
    ax.set_xlabel("n ключів")
    ax.set_ylabel("нс на пошук")
    ax.set_title("Час пошуку")
    ax.margins(x=0.45)
    finish(fig, "bst_avl.png")


def plot_skiplist():
    rows = read("skiplist.csv")
    if not rows:
        return
    d = defaultdict(lambda: ([], [], [], []))
    for r in rows:
        p = float(r["p"])
        d[p][0].append(int(r["n"]))
        d[p][1].append(float(r["steps_per_search"]))
        d[p][2].append(float(r["theory_steps"]))
        d[p][3].append(float(r["pointers_per_node"]))
    fig, axes = plt.subplots(1, 2, figsize=(15, 5.4), gridspec_kw={"width_ratios": [2.2, 1]})
    ax = axes[0]
    names = {0.5: "p = 1/2", 0.25: "p = 1/4", 0.3679: "p = 1/e", 0.125: "p = 1/8"}
    for i, p in enumerate(sorted(d, reverse=True)):
        line(ax, d[p][0], d[p][1], i, f"{names.get(round(p, 4), p)} виміряно")
        ax.plot(d[p][0], d[p][2], color=SERIES[i], lw=1, ls=":", alpha=0.9)
    ax.set_xscale("log", base=2)
    ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
    ax.set_xlabel("n")
    ax.set_ylabel("переходів по вказівниках на пошук")
    ax.set_title("Skip list: кроки пошуку (пунктир — верхня межа Пью 1990)")
    ax.margins(x=0.3)
    ax = axes[1]
    ps = sorted(d, reverse=True)
    vals = [d[p][3][-1] for p in ps]
    bars = ax.bar(range(len(ps)), vals, color=[SERIES[i] for i in range(len(ps))], width=0.6, edgecolor=SURFACE, linewidth=2)
    for i, (b, p) in enumerate(zip(bars, ps)):
        ax.text(b.get_x() + b.get_width() / 2, b.get_height() + 0.03, f"{vals[i]:.3f}\n(теорія {1 / (1 - p):.3f})",
                ha="center", fontsize=8.5, color=INK)
    ax.set_xticks(range(len(ps)), [names.get(round(p, 4), str(p)) for p in ps])
    ax.set_ylabel("вказівників next на вузол")
    ax.set_ylim(0, max(vals) * 1.3)
    ax.set_title("Пам'ять: 1/(1−p)")
    ax.grid(axis="x", visible=False)
    finish(fig, "skiplist.png")


def plot_heap():
    rows = read("heap_build.csv")
    if not rows:
        return
    d = defaultdict(lambda: ([], []))
    for r in rows:
        d[r["method"]][0].append(int(r["n"]))
        d[r["method"]][1].append(float(r["ns_per_elem"]))
    fig, ax = plt.subplots(figsize=(11, 5.6))
    for i, m in enumerate(["floyd_build_random", "floyd_build_descending", "push_n_times_random", "push_n_times_descending"]):
        line(ax, d[m][0], d[m][1], i, m)
    ax.set_xscale("log", base=2)
    ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
    ax.set_xlabel("n")
    ax.set_ylabel("нс на елемент")
    ax.set_title("Побудова мін-купи: Флойд Θ(n) vs n × push Θ(n log n) у гіршому випадку")
    ax.margins(x=0.35)
    finish(fig, "heap_build.png")


def plot_graph():
    a = read("graph_bfs.csv")
    b = read("graph_density.csv")
    if not a or not b:
        return
    fig, axes = plt.subplots(1, 2, figsize=(15, 5.4))
    ax = axes[0]
    V = [int(r["V"]) for r in a]
    line(ax, V, [int(r["ns_csr"]) / 1e3 for r in a], 0, "CSR")
    line(ax, V, [int(r["ns_matrix"]) / 1e3 for r in a], 1, "матриця суміжності")
    ax.set_xscale("log", base=2)
    ax.set_yscale("log")
    ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
    ax.set_xlabel("V (середній степінь 8, E = 4V)")
    ax.set_ylabel("мкс на BFS")
    ax.set_title("BFS, розріджений граф: Θ(V+E) vs Θ(V²)")
    ax.margins(x=0.3)
    ax = axes[1]
    E = [int(r["E"]) for r in b]
    line(ax, E, [int(r["ns_csr"]) / 1e3 for r in b], 0, "CSR")
    line(ax, E, [int(r["ns_matrix"]) / 1e3 for r in b], 1, "матриця")
    ax.set_xscale("log", base=2)
    ax.set_yscale("log")
    ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
    ax.set_xlabel("E (V = 4096 фіксоване)")
    ax.set_ylabel("мкс на BFS")
    ax.set_title("BFS при зростанні щільності")
    ax.margins(x=0.25)
    finish(fig, "graph_bfs.png")


# ------------------------------------------------------------------ errata

def plot_errata():
    rows = read(os.path.join(ROOT, "errata", "errata.csv"))
    if not rows:
        return
    chapters = {}
    for r in rows:
        if r["verdict"] == "REJECTED":
            continue
        key = (int(r["chapter_no"]), r["chapter"])
        c = chapters.setdefault(key, {"ERROR": 0, "IMPRECISE": 0})
        c[r["severity"]] += 1
    keys = sorted(chapters)
    labels = [f"{n:02d} {name}" for n, name in keys]
    err = [chapters[k]["ERROR"] for k in keys]
    imp = [chapters[k]["IMPRECISE"] for k in keys]
    fig, ax = plt.subplots(figsize=(11, 7))
    y = range(len(keys))
    ax.barh(y, err, color=SERIES[7], label="ПОМИЛКА (однозначно неправильно)", edgecolor=SURFACE, linewidth=2, height=0.7)
    ax.barh(y, imp, left=err, color=SERIES[0], label="НЕТОЧНІСТЬ (вводить в оману)", edgecolor=SURFACE, linewidth=2,
            height=0.7)
    for i in y:
        ax.text(err[i] + imp[i] + 0.4, i, f"{err[i]} + {imp[i]} = {err[i] + imp[i]}", va="center", fontsize=8.5)
    ax.set_yticks(list(y), labels)
    ax.invert_yaxis()
    ax.grid(axis="y", visible=False)
    ax.set_xlabel("кількість підтверджених зауважень")
    total_e, total_i = sum(err), sum(imp)
    rej = sum(1 for r in rows if r["verdict"] == "REJECTED")
    ax.set_title(f"Знайдені помилки за розділами: {total_e} помилок + {total_i} неточностей "
                 f"(відхилено кандидатів: {rej})")
    ax.legend(loc="upper right")
    ax.set_xlim(0, max(e + i for e, i in zip(err, imp)) * 1.2)
    finish(fig, "errata_by_chapter.png", "Джерело: errata/errata.csv (кожен пункт перевірено за зображенням сторінки)")


if __name__ == "__main__":
    for fn in (plot_sort_time, plot_sort_heatmap, plot_sort_ops, plot_search_time, plot_search_ops, plot_traverse,
               plot_growth, plot_bst_avl, plot_skiplist, plot_heap, plot_graph, plot_errata):
        fn()
