// 10 — Prime Number Within a Given Range | v1: partition, each rank prints
// Every rank scans its contiguous chunk of [L..R] and prints the primes it
// finds (tagged with its rank). NOTE the load imbalance: is_prime(x) costs
// O(sqrt(x)) so the later chunks are more expensive — exactly the situation
// work stealing (see LoadBalancing/) was invented for.
// Run: make run PROB=10_prime_in_range SRC=result/v1_print.cpp NP=4 ARGS="2 200"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    for (uint64_t d = 2; d * d <= n; ++d)
        if (n % d == 0) return false;
    return true;
}

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

    for (uint64_t x = my_lo; x <= my_hi; ++x)
        if (is_prime(x))
            printf("[rank %llu] %llu\n", me, x);

    BCL::finalize();
    return 0;
}
