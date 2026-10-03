#!/usr/bin/env python3
"""Prints every number from results/*.csv that the documentation (docs/, README.md) refers to.

Usage:  .venv/bin/python scripts/doc_facts.py [csv_dir]
After `make bench`, diff the output against the old values to update the texts:
  python3 scripts/doc_facts.py /tmp/old > old.txt; python3 scripts/doc_facts.py > new.txt; diff old.txt new.txt
"""
import csv
import math
import os
import sys

RES = sys.argv[1] if len(sys.argv) > 1 else os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "results")


def rows(name):
    with open(os.path.join(RES, name), newline="") as f:
        return list(csv.DictReader(f))


def get(name, col, **kw):
    for r in rows(name):
        if all(str(r[k]) == str(v) for k, v in kw.items()):
            return float(r[col])
    return float("nan")


def out(label, v, fmt="{:.2f}"):
    print(f"{label:<60} {fmt.format(v)}")


st = lambda a, inp, n: get("sort_time.csv", "ns_per_elem", algo=a, input=inp, n=n)
print("## sort_time (ns/elem)")
for a in ("radix_lsd", "counting", "bucket", "merge_topdown", "quick_median3", "heap"):
    out(f"{a} random 4M", st(a, "random", 4194304))
for a in ("heap", "merge_topdown", "quick_median3", "quick_hoare_mid", "quick_3way", "libc_qsort", "shell_ciura"):
    out(f"{a} random 2M", st(a, "random", 2097152))
out("libc_qsort random 65536", st("libc_qsort", "random", 65536))
out("quick_median3 random 65536", st("quick_median3", "random", 65536))
out("insertion / insertion_binary random 65536", st("insertion", "random", 65536) / st("insertion_binary", "random", 65536))
out("bucket few_unique 4M", st("bucket", "few_unique", 4194304))
out("quick_lomuto_last few_unique 16384", st("quick_lomuto_last", "few_unique", 16384))
out("quick_3way few_unique 16384", st("quick_3way", "few_unique", 16384))
print("## sort_time: faster than heap at 2M?")
h = st("heap", "random", 2097152)
print("   slower than heap:", [r["algo"] for r in rows("sort_time.csv")
                               if r["input"] == "random" and int(r["n"]) == 2097152 and float(r["ns_per_elem"]) > h])

print("## search_time (ns/query)")
for n in (16, 1024, 65536, 1048576, 16777216):
    for a in ("linear", "binary", "branchless", "eytzinger", "eytzinger_unclamped", "interpolation"):
        v = get("search_time.csv", "ns_per_query", algo=a, n=n)
        if not math.isnan(v):
            out(f"{a} n={n}", v)
out("binary/eytzinger 16M", get("search_time.csv", "ns_per_query", algo="binary", n=16777216)
    / get("search_time.csv", "ns_per_query", algo="eytzinger", n=16777216))

print("## search_ops (deterministic)")
for d in ("arithmetic", "uniform_random", "skewed_x4"):
    for a in ("binary", "interpolation", "jump", "exponential", "ternary"):
        out(f"{a} {d} 2^20 avg/max", get("search_ops.csv", "avg_cmp", algo=a, data=d, n=1048576))

print("## traverse (ns/elem)")
for n in (256, 1024, 65536, 1048576, 16777216):
    for l in ("array", "list_sequential", "list_shuffled"):
        out(f"{l} n={n}", get("traverse.csv", "ns_per_elem", layout=l, n=n), "{:.4f}")
for b in (131072, 262144, 16777216, 33554432, 67108864):
    out(f"list_shuffled footprint={b}", get("traverse.csv", "ns_per_elem", layout="list_shuffled", bytes_footprint=b))
out("ratio shuffled/array 16M", get("traverse.csv", "ns_per_elem", layout="list_shuffled", n=16777216)
    / get("traverse.csv", "ns_per_elem", layout="array", n=16777216), "{:.0f}")

print("## stack_growth (ns/push)")
for p in ("memcpy_double_x2", "memcpy_linear_+1024"):
    for n in (65536, 1048576):
        out(f"{p} n={n}", get("stack_growth.csv", "ns_per_push", policy=p, n=n), "{:.3f}")
        out(f"{p} n={n} copied", get("stack_growth.csv", "reallocs_or_copied", policy=p, n=n), "{:.0f}")

print("## bst_avl")
for n in (1024, 32768, 1048576):
    for t in ("bst", "avl"):
        for i in ("random", "sorted"):
            h_ = get("bst_avl.csv", "height", tree=t, input=i, n=n)
            if math.isnan(h_):
                continue
            print(f"{t} {i} n={n}: height={h_:.0f} depth={get('bst_avl.csv', 'avg_depth', tree=t, input=i, n=n):.2f} "
                  f"search={get('bst_avl.csv', 'ns_per_search', tree=t, input=i, n=n):.2f} "
                  f"rot={get('bst_avl.csv', 'rotations_per_insert', tree=t, input=i, n=n):.4f}")
out("bst/avl sorted search 32768", get("bst_avl.csv", "ns_per_search", tree="bst", input="sorted", n=32768)
    / get("bst_avl.csv", "ns_per_search", tree="avl", input="sorted", n=32768), "{:.0f}")

print("## skiplist (n≈2^20)")
for r in rows("skiplist.csv"):
    if int(r["n"]) > 1000000:
        print(f"p={r['p']}: ptr={float(r['pointers_per_node']):.3f} steps={float(r['steps_per_search']):.1f} "
              f"bound={float(r['theory_steps']):.1f} ns={float(r['ns_per_search']):.0f}")

print("## heap_build 2^24 (ns/elem)")
for m in ("floyd_build_random", "push_n_times_random", "floyd_build_descending", "push_n_times_descending"):
    out(m, get("heap_build.csv", "ns_per_elem", method=m, n=16777216))

print("## graph")
for V in (1024, 16384):
    out(f"csr V={V} us", get("graph_bfs.csv", "ns_csr", V=V) / 1e3, "{:.1f}")
    out(f"matrix V={V} us", get("graph_bfs.csv", "ns_matrix", V=V) / 1e3, "{:.1f}")
out("matrix/csr V=16384", get("graph_bfs.csv", "ns_matrix", V=16384) / get("graph_bfs.csv", "ns_csr", V=16384), "{:.0f}")
for r in rows("graph_density.csv")[-2:]:
    print(f"density E={r['E']}: csr={int(r['ns_csr']) / 1e6:.2f} ms matrix={int(r['ns_matrix']) / 1e6:.2f} ms")
