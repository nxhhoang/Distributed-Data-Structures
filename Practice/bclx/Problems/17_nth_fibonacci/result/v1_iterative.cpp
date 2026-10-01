// 17 — Nth Term of the Fibonacci Series | v1: iterative on rank 0
// Convention: F(0) = 0, F(1) = 1. F(93) is the largest term fitting uint64.
// Run: make run PROB=17_nth_fibonacci SRC=result/v1_iterative.cpp NP=4 ARGS="50"

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
    if (n > 93) n = 93;

    uint64_t a = 0, b = 1;
    for (uint64_t i = 0; i < n; ++i) { uint64_t c = a + b; a = b; b = c; }

    if (BCL::rank() == 0)
        printf("F(%llu) = %llu\n", n, a);

    BCL::finalize();
    return 0;
}
