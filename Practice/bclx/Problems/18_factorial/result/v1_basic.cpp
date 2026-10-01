// 18 — Factorial of a Number | v1: iterative on rank 0
// Note: 20! is the largest factorial that fits in uint64.
// Run: make run PROB=18_factorial SRC=result/v1_basic.cpp NP=4 ARGS="20"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n > 20) {
        if (BCL::rank() == 0) fprintf(stderr, "warning: clamping to 20 (uint64 overflow)\n");
        n = 20;
    }

    uint64_t f = 1;
    for (uint64_t i = 2; i <= n; ++i) f *= i;

    if (BCL::rank() == 0)
        printf("%llu! = %llu\n", n, f);

    BCL::finalize();
    return 0;
}
