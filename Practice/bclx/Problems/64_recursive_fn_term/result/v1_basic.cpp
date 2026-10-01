// 64 — Print the F(N)th Term (recursive sequence) | v1: plain recursion
// Sequence: t(1) = 1, t(2) = 1, t(n) = t(n-1)^2 + t(n-2)^2
//   -> 1, 1, 2, 5, 29, 866, 750797, 563696385365 ...
// t(9) overflows uint64, so N is clamped to 8.
// Run: make run PROB=64_recursive_fn_term SRC=result/v1_basic.cpp NP=4 ARGS="6"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t t(uint64_t n) {
    return n <= 2 ? 1 : t(n - 1) * t(n - 1) + t(n - 2) * t(n - 2);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <N>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n > 8) {
        if (BCL::rank() == 0) fprintf(stderr, "warning: clamping N to 8 (uint64 overflow)\n");
        n = 8;
    }

    if (BCL::rank() == 0)
        printf("F(%llu) = %llu (recursion)\n", n, t(n));

    BCL::finalize();
    return 0;
}
