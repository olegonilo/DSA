#!/usr/bin/env python3
"""Builds all charts/*.png EXCLUSIVELY from results/*.csv and errata/errata.csv.

No number on the charts is hard-coded: if a CSV is missing, the chart is skipped with a message.
The palette is a validated categorical set (CVD-safe for adjacent pairs); series identity is
also encoded by marker, line style and a label at the end of the line (not by color alone).
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
    for unit, k in (("GB", 1 << 30), ("MB", 1 << 20), ("KB", 1 << 10)):
        if x >= k:
            v = x / k
            return f"{v:.0f} {unit}" if v >= 10 or v == int(v) else f"{v:.1f} {unit}"
    return f"{x:.0f} B"


def n_fmt(x, _pos=None):
    if x >= 1 << 20:
        return f"{x / (1 << 20):.0f}M" if x % (1 << 20) == 0 else f"{x / (1 << 20):.1f}M"
    if x >= 1 << 10:
        return f"{x / (1 << 10):.0f}K"
    return f"{x:.0f}"


_PENDING = defaultdict(list)


def end_label(ax, xs, ys, text, color, dy=0):
    """Label at the end of a line. Placement is deferred to finish(), where the labels of one axis
    are spread vertically so they do not overlap."""
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
        gap = 13.0  # pixels between labels
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
    """Color is bound to the entity (algorithm), not to its position in the legend."""
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
    for size, name in ((128 << 10, "L1d 128 KB"), (16 << 20, "L2 16 MB")):
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
        ("Θ(n²): time per element grows linearly with n",
         ["bubble_naive", "bubble", "selection", "insertion", "insertion_binary"]),
        ("O(n log n) and Shell: time per element grows ~log n",
         ["shell_ciura", "merge_topdown", "quick_median3", "quick_3way", "heap", "libc_qsort"]),
        ("Non-comparison (keys in [0, n)) vs quick_median3",
         ["counting", "radix_lsd", "bucket", "quick_median3"]),
    ]
    fig, axes = plt.subplots(1, 3, figsize=(17, 5.4))
    for ax, (title, algos) in zip(axes, groups):
        for i, a in enumerate(algos):
            xs, ys = data[a]
            # quick_median3 - same color on both panels; non-comparison sorts get their own slots
            slot = {"quick_median3": 2, "counting": 6, "radix_lsd": 7, "bucket": 4}.get(a, i) if ax is axes[2] else i
            line(ax, xs, ys, slot, a)
        ax.set_xscale("log", base=2)
        ax.set_yscale("log")
        ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
        ax.set_title(title, fontsize=10.5)
        ax.set_xlabel("n (random ints)")
        ax.set_ylabel("ns per element (median)")
        ax.margins(x=0.25)
    fig.suptitle("Sorting: measured time / n", x=0.01, ha="left", fontweight="bold")
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
    ax.set_title(f"Sorting, n = {N}: ns per element (darker = slower, log scale)")
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
    # Lower bound for comparison sorts: ceil(log2(n!)) comparisons in the worst case
    # (and log2(n!) - O(1) on average). log2(n!) = n·log2 n − 1.4427·n + O(log n) < n·log2 n.
    xs = d[("merge_topdown", "random")][0]
    lb = [math.lgamma(n + 1) / math.log(2) / (n * math.log2(n)) for n in xs]
    ax.plot(xs, lb, color=INK2, lw=1.2, ls=":")
    end_label(ax, xs, lb, "log₂(n!) — lower bound", INK2)
    ax.set_xscale("log", base=2)
    ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
    ax.set_title("Comparisons / (n·log₂n), random input")
    ax.set_xlabel("n")
    ax.set_ylabel("cmp / (n log₂ n)")
    ax.margins(x=0.25)
    ax = axes[1]
    combos = [("insertion", "random", "insertion · random → n²/4"),
              ("insertion", "reversed", "insertion · reversed → n²/2"),
              ("selection", "sorted", "selection · any → n²/2"),
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
    ax.set_title("Comparisons / n²: the n² constant (exact counters)")
    ax.set_xlabel("n")
    ax.set_ylabel("cmp / n²")
    ax.margins(x=0.45)
    finish(fig, "sort_ops.png", "Exact comparison counts (deterministic, -DDSA_COUNT) — machine-independent")


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
    ax.set_xlabel("array size (sorted uniformly random ints)")
    ax.set_ylabel("ns per successful search (median of 5 × 2²⁰ queries)")
    ax.set_title("Search: same O(log n), different interaction with the cache and branch predictor")
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
              ("interpolation", "arithmetic", "interpolation · arithmetic progression"),
              ("interpolation", "uniform_random", "interpolation · uniform random"),
              ("interpolation", "skewed_x4", "interpolation · skewed (x⁴)"),
              ("jump", "uniform_random", "jump (√n)"), ("ternary", "uniform_random", "ternary")]
    for i, (a, dat, lab) in enumerate(combos):
        xs, ys = d[(a, dat)]
        line(ax, xs, ys, i, lab)
    ax.set_xscale("log", base=2)
    ax.set_yscale("log")
    ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
    ax.set_xlabel("n")
    ax.set_ylabel("average number of key comparisons")
    ax.set_title("Search: comparison count (successful search, 2000 random keys)")
    ax.margins(x=0.3)
    finish(fig, "search_ops.png", "Exact counters (-DDSA_COUNT). Ternary does FEWER iterations but MORE comparisons than binary")


# ------------------------------------------------------------------ data structures

def plot_traverse():
    rows = read("traverse.csv")
    if not rows:
        return
    d = defaultdict(lambda: ([], []))
    for r in rows:  # bytes_footprint: int = 4 B, sll_node = 16 B — the actual footprint of the structure
        d[r["layout"]][0].append(int(r["bytes_footprint"]))
        d[r["layout"]][1].append(float(r["ns_per_elem"]))
    fig, ax = plt.subplots(figsize=(11, 6))
    labels = {"array": "int array", "list_sequential": "list, contiguous nodes",
              "list_shuffled": "list, shuffled nodes"}
    for i, k in enumerate(["array", "list_sequential", "list_shuffled"]):
        line(ax, d[k][0], d[k][1], i, labels[k])
    cache_lines(ax)
    ax.set_xscale("log", base=2)
    ax.set_yscale("log")
    ax.xaxis.set_major_formatter(FuncFormatter(bytes_fmt))
    ax.set_xlabel("memory footprint of the structure")
    ax.set_ylabel("ns per element traversed (sum of all elements)")
    ratio = d["list_shuffled"][1][-1] / d["array"][1][-1]
    ax.set_title(f"Traversal is Θ(n) in all three cases, but at large n the shuffled list is "
                 f"{ratio:.0f}× slower than the array")
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
        ("Explicit copying (malloc + memcpy + free) — as in theory", ["memcpy_double_x2", "memcpy_linear_+1024"]),
        ("realloc() on macOS — large blocks grow in place (see ex_realloc_inplace)", ["double_x2", "linear_+1024", "linear_+64"]),
    ]):
        for i, p in enumerate(pols):
            line(ax, d[p][0], d[p][1], i, p)
        ax.set_xscale("log", base=2)
        ax.set_yscale("log")
        ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
        ax.set_title(title, fontsize=10.5)
        ax.set_xlabel("n pushes")
        ax.margins(x=0.3)
    axes[0].set_ylabel("ns per push (amortized)")
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
    labels = {("bst", "random"): "BST · random order", ("bst", "sorted"): "BST · sorted (degenerate)",
              ("avl", "random"): "AVL · random", ("avl", "sorted"): "AVL · sorted"}
    for i, k in enumerate(labels):
        line(ax, d[k][0], d[k][1], i, labels[k])
    ns = d[("avl", "random")][0]
    ax.plot(ns, [math.log2(n + 1) for n in ns], color=INK2, lw=1, ls=":")
    ax.plot(ns, [1.4405 * math.log2(n + 2) - 0.3277 for n in ns], color=INK2, lw=1, ls="--")
    end_label(ax, ns, [math.log2(n + 1) for n in ns], "log₂(n+1) — minimum", INK2, -8)
    end_label(ax, ns, [1.4405 * math.log2(n + 2) - 0.3277 for n in ns], "1.44·log₂(n+2) — AVL bound", INK2, 8)
    ax.set_xscale("log", base=2)
    ax.set_yscale("log", base=2)
    ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
    ax.set_xlabel("n keys")
    ax.set_ylabel("height (number of levels)")
    ax.set_title("Tree height")
    ax.margins(x=0.45)
    ax = axes[1]
    for i, k in enumerate(labels):
        line(ax, d[k][0], d[k][2], i, labels[k])
    ax.set_xscale("log", base=2)
    ax.set_yscale("log")
    ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
    ax.set_xlabel("n keys")
    ax.set_ylabel("ns per search")
    ax.set_title("Search time")
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
        line(ax, d[p][0], d[p][1], i, f"{names.get(round(p, 4), p)} measured")
        ax.plot(d[p][0], d[p][2], color=SERIES[i], lw=1, ls=":", alpha=0.9)
    ax.set_xscale("log", base=2)
    ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
    ax.set_xlabel("n")
    ax.set_ylabel("pointer hops per search")
    ax.set_title("Skip list: search steps (dotted — Pugh 1990 upper bound)")
    ax.margins(x=0.3)
    ax = axes[1]
    ps = sorted(d, reverse=True)
    vals = [d[p][3][-1] for p in ps]
    bars = ax.bar(range(len(ps)), vals, color=[SERIES[i] for i in range(len(ps))], width=0.6, edgecolor=SURFACE, linewidth=2)
    for i, (b, p) in enumerate(zip(bars, ps)):
        ax.text(b.get_x() + b.get_width() / 2, b.get_height() + 0.03, f"{vals[i]:.3f}\n(theory {1 / (1 - p):.3f})",
                ha="center", fontsize=8.5, color=INK)
    ax.set_xticks(range(len(ps)), [names.get(round(p, 4), str(p)) for p in ps])
    ax.set_ylabel("next pointers per node")
    ax.set_ylim(0, max(vals) * 1.3)
    ax.set_title("Memory: 1/(1−p)")
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
    ax.set_ylabel("ns per element")
    ax.set_title("Min-heap construction: Floyd Θ(n) vs n × push Θ(n log n) worst case")
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
    line(ax, V, [int(r["ns_matrix"]) / 1e3 for r in a], 1, "adjacency matrix")
    ax.set_xscale("log", base=2)
    ax.set_yscale("log")
    ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
    ax.set_xlabel("V (average degree 8, E = 4V)")
    ax.set_ylabel("µs per BFS")
    ax.set_title("BFS, sparse graph: Θ(V+E) vs Θ(V²)")
    ax.margins(x=0.3)
    ax = axes[1]
    E = [int(r["E"]) for r in b]
    line(ax, E, [int(r["ns_csr"]) / 1e3 for r in b], 0, "CSR")
    line(ax, E, [int(r["ns_matrix"]) / 1e3 for r in b], 1, "matrix")
    ax.set_xscale("log", base=2)
    ax.set_yscale("log")
    ax.xaxis.set_major_formatter(FuncFormatter(n_fmt))
    ax.set_xlabel("E (fixed V = 4096)")
    ax.set_ylabel("µs per BFS")
    ax.set_title("BFS with growing density")
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
    ax.barh(y, err, color=SERIES[7], label="ERROR (definitely wrong)", edgecolor=SURFACE, linewidth=2, height=0.7)
    ax.barh(y, imp, left=err, color=SERIES[0], label="IMPRECISE (misleading)", edgecolor=SURFACE, linewidth=2,
            height=0.7)
    for i in y:
        ax.text(err[i] + imp[i] + 0.4, i, f"{err[i]} + {imp[i]} = {err[i] + imp[i]}", va="center", fontsize=8.5)
    ax.set_yticks(list(y), labels)
    ax.invert_yaxis()
    ax.grid(axis="y", visible=False)
    ax.set_xlabel("number of confirmed issues")
    total_e, total_i = sum(err), sum(imp)
    rej = sum(1 for r in rows if r["verdict"] == "REJECTED")
    ax.set_title(f"Issues found by chapter: {total_e} errors + {total_i} imprecisions "
                 f"(candidates rejected: {rej})")
    ax.legend(loc="upper right")
    ax.set_xlim(0, max(e + i for e, i in zip(err, imp)) * 1.2)
    finish(fig, "errata_by_chapter.png", "Source: errata/errata.csv (each item verified against the page image)")


if __name__ == "__main__":
    for fn in (plot_sort_time, plot_sort_heatmap, plot_sort_ops, plot_search_time, plot_search_ops, plot_traverse,
               plot_growth, plot_bst_avl, plot_skiplist, plot_heap, plot_graph, plot_errata):
        fn()
