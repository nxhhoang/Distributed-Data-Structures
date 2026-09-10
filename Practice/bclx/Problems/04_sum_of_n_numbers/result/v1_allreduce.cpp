// 04 — Sum of N Numbers | v1: partition the array + allreduce
// The N numbers are generated deterministically from their index:
// a[i] = (i * 2654435761 + 17) % 1000 — so no data movement is needed to
// create them, and every rank can regenerate its own chunk.
// Run: make run PROB=04_sum_of_n_numbers SRC=result/v1_allreduce.cpp NP=4 ARGS="1000000"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 1000;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <N>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t n = 0;
    if (me == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);

    uint64_t lo = n * me / P;          // my chunk of indices [lo, hi)
    uint64_t hi = n * (me + 1) / P;

    uint64_t partial = 0;
    for (uint64_t i = lo; i < hi; ++i) partial += value_at(i);

    uint64_t total = bclx::allreduce(partial, BCL::sum<uint64_t>{});

    if (me == 0)
        printf("sum of %llu numbers = %llu\n", n, total);

    BCL::finalize();
    return 0;
}
