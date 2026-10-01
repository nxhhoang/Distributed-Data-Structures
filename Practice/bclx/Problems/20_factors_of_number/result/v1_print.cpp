// 20 — Factor of a Number | v1: partition [1..n], each rank prints its finds
// Every rank scans its chunk and prints the divisors it discovers (tagged).
// Run: make run PROB=20_factors_of_number SRC=result/v1_print.cpp NP=4 ARGS="100"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t n = 0;
    if (me == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);

    // contiguous chunks over the candidate range [1..n]
    uint64_t my_lo = n * me / P + 1;
    uint64_t my_hi = n * (me + 1) / P;

    for (uint64_t d = my_lo; d <= my_hi; ++d)
        if (n % d == 0)
            printf("[rank %llu] %llu divides %llu\n", me, d, n);

    BCL::finalize();
    return 0;
}
