// 69 — Factorial Using Recursion | v1: plain recursion
// Same quantity as 18/v1 (iterative) — this is its recursive twin; see
// 18/v2 for the distributed chain variant.
// Run: make run PROB=69_recursive_factorial SRC=result/v1_basic.cpp NP=4 ARGS="10"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t fact(uint64_t n) {
    return n <= 1 ? 1 : n * fact(n - 1);
}

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

    if (BCL::rank() == 0)
        printf("%llu! = %llu (recursion)\n", n, fact(n));

    BCL::finalize();
    return 0;
}
