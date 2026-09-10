// 03 — Sum of First N Natural Numbers | v1: closed form on rank 0
// N*(N+1)/2. Note: fits in uint64 for N <= 4294967294 (n*(n+1) < 2^64).
// Run: make run PROB=03_sum_of_first_n_natural SRC=result/v1_formula.cpp NP=4 ARGS="100"
#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <N>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);

    uint64_t sum = n * (n + 1) / 2;
    if (BCL::rank() == 0)
        printf("1 + 2 + ... + %llu = %llu\n", n, sum);

    BCL::finalize();
    return 0;
}
