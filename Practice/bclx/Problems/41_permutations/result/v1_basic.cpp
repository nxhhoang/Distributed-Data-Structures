// 41 — Permutations: n People Occupy r Seats | v1: falling product
// P(n, r) = n * (n-1) * ... * (n-r+1) — computed as a falling product instead
// of n!/(n-r)! to delay overflow.
// Run: make run PROB=41_permutations SRC=result/v1_basic.cpp NP=4 ARGS="10 3"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n> <r>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0, r = 0;
    if (BCL::rank() == 0) {
        n = strtoull(argv[1], nullptr, 10);
        r = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    r = BCL::broadcast(r, 0);

    uint64_t p = (r > n) ? 0 : 1;   // guard: P(n, r) = 0 when r > n
    for (uint64_t i = 0; i < r; ++i)
        p *= (n - i);

    if (BCL::rank() == 0)
        printf("P(%llu, %llu) = %llu\n", n, r, p);

    BCL::finalize();
    return 0;
}
