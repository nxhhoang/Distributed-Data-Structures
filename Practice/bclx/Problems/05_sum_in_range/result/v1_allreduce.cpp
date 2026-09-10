// 05 — Sum of Numbers in a Given Range | v1: partition [L..R] + allreduce
// Run: make run PROB=05_sum_in_range SRC=result/v1_allreduce.cpp NP=4 ARGS="10 1000000"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <L> <R>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t lo = 0, hi = 0;
    if (me == 0) {
        lo = strtoull(argv[1], nullptr, 10);
        hi = strtoull(argv[2], nullptr, 10);
    }
    lo = BCL::broadcast(lo, 0);
    hi = BCL::broadcast(hi, 0);

    uint64_t len = hi - lo + 1;
    uint64_t my_lo = lo + me * len / P;
    uint64_t my_hi = lo + (me + 1) * len / P - 1;

    uint64_t partial = 0;
    for (uint64_t x = my_lo; x <= my_hi; ++x) partial += x;

    uint64_t total = bclx::allreduce(partial, BCL::sum<uint64_t>{});

    if (me == 0)
        printf("sum(%llu..%llu) = %llu\n", lo, hi, total);

    BCL::finalize();
    return 0;
}
