# DSA — build with no external dependencies (only a C11 compiler and make).
#
#   make test         — unit tests under AddressSanitizer + UBSan
#   make bench        — all benchmarks -> results/*.csv
#   make plots        — charts results/*.csv -> charts/*.png (requires .venv with matplotlib)
#   make experiments  — demonstrations of errors in the notes (before/after)
#   make all          — test + experiments + bench + plots

CC      ?= cc
STD     := -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wno-sign-conversion -Iinclude
OPT     := -O2 -march=native
SAN     := -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all
LDLIBS  := -lm
PY      ?= .venv/bin/python

SRC     := src/common.c src/sort.c src/search.c src/list.c src/skiplist.c \
           src/stack_queue.c src/tree.c src/graph.c
TESTS   := $(patsubst tests/%.c,build/tests/%,$(wildcard tests/test_*.c))
BENCHES := $(patsubst bench/%.c,build/bench/%,$(wildcard bench/bench_*.c))
EXPS    := $(patsubst experiments/%.c,build/exp/%,$(wildcard experiments/ex_*.c))

.PHONY: all test bench plots experiments clean

all: test experiments bench plots

build/tests/%: tests/%.c $(SRC) tests/minitest.h
	@mkdir -p $(@D)
	$(CC) $(STD) $(SAN) -DDSA_COUNT $< $(SRC) -o $@ $(LDLIBS)

# Timing benchmarks: WITHOUT counters (-DDSA_COUNT not set), so the increments do not distort timings.
build/bench/%: bench/%.c $(SRC) bench/bench_util.h
	@mkdir -p $(@D)
	$(CC) $(STD) $(OPT) $< $(SRC) -o $@ $(LDLIBS)

# bench_ops counts operations — built with counters.
build/bench/bench_ops: bench/bench_ops.c $(SRC) bench/bench_util.h
	@mkdir -p $(@D)
	$(CC) $(STD) $(OPT) -DDSA_COUNT $< $(SRC) -o $@ $(LDLIBS)

# Experiments are built with -O0 so the compiler does not "optimize away" the demonstrated behavior,
# except those that compare performance (they state how they should be built — see EXP_O2).
# ex_ub_* reproduce undefined behavior from the notes — built with ASan/UBSan so you can SEE it.
EXP_O2  := build/exp/ex_struct_padding build/exp/ex_realloc_inplace build/exp/ex_small_n_crossover
build/exp/ex_ub_%: experiments/ex_ub_%.c experiments/exp_util.h
	@mkdir -p $(@D)
	$(CC) $(STD) -Wno-vla -O0 -g -fsanitize=address,undefined -fno-omit-frame-pointer $< -o $@
build/exp/%: experiments/%.c $(SRC)
	@mkdir -p $(@D)
	$(CC) $(STD) $(if $(filter $@,$(EXP_O2)),$(OPT),-O0 -g) $< $(SRC) -o $@ $(LDLIBS)

test: $(TESTS)
	@set -e; for t in $(TESTS); do echo "== $$t"; ./$$t; done

bench: $(BENCHES)
	@mkdir -p results
	@set -e; for b in $(BENCHES); do echo "== $$b"; ./$$b; done

experiments: $(EXPS)
	@set -e; for e in $(EXPS); do echo; echo "================ $$e"; ./$$e; done

plots:
	$(PY) scripts/plot.py

clean:
	rm -rf build
