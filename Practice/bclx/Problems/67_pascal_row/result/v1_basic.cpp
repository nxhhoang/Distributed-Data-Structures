// 67 — Nth Row of Pascal's Triangle | v1: recursive binomial coefficients
// C(n, k) = C(n-1, k-1) + C(n-1, k) — the textbook recursion (exponential,
// but fine for small n).
// Run: make run PROB=67_pascal_row SRC=result/v1_basic.cpp NP=4 ARGS="6"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t C(uint64_t n, uint64_t k) {
    if (k == 0 || k == n) return 1;
    return C(n - 1, k - 1) + C(n - 1, k);
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
    if (n > 25) n = 25;   // keep the exponential recursion + uint64 sane

    if (BCL::rank() == 0) {
        printf("row %llu of Pascal's triangle: ", n);
        for (uint64_t k = 0; k <= n; ++k)
            printf("%llu ", C(n, k));
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
