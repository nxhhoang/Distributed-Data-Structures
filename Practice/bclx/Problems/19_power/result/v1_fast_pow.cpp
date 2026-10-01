// 19 — Power of a Number | v1: fast exponentiation (square-and-multiply)
// b^e in O(log e). Note: assumes b^e fits in uint64 — the caller's business
// here (the same problem the SMM workload had).
// Run: make run PROB=19_power SRC=result/v1_fast_pow.cpp NP=4 ARGS="2 40"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <base> <exp>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t b = 0, e = 0;
    if (BCL::rank() == 0) {
        b = strtoull(argv[1], nullptr, 10);
        e = strtoull(argv[2], nullptr, 10);
    }
    b = BCL::broadcast(b, 0);
    e = BCL::broadcast(e, 0);

    uint64_t result = 1;
    while (e > 0) {
        if (e & 1) result *= b;
        b *= b;
        e >>= 1;
    }

    if (BCL::rank() == 0)
        printf("result = %llu\n", result);

    BCL::finalize();
    return 0;
}
