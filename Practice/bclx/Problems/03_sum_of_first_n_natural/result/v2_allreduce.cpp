// 03 — Sum of First N Natural Numbers | v2: distributed partition + allreduce
// Every rank sums its contiguous chunk of [1..N]; partials are combined with
// bclx::allreduce. Rank 0 cross-checks against the closed form N*(N+1)/2 —
// the "distributed result must match the formula" habit.
// Run: make run PROB=03_sum_of_first_n_natural SRC=result/v2_allreduce.cpp NP=4 ARGS="1000000"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

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

    // my chunk of [1..n]:  [n*me/P + 1 .. n*(me+1)/P]
    uint64_t lo = n * me / P + 1;
    uint64_t hi = n * (me + 1) / P;

    uint64_t partial = 0;
    for (uint64_t x = lo; x <= hi; ++x) partial += x;

    uint64_t total = bclx::allreduce(partial, BCL::sum<uint64_t>{});

    if (me == 0) {
        uint64_t formula = n * (n + 1) / 2;
        printf("distributed sum = %llu, formula = %llu -> %s\n",
               total, formula, total == formula ? "MATCH" : "MISMATCH!");
    }

    BCL::finalize();
    return 0;
}
