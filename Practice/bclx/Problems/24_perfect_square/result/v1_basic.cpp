// 24 — Perfect Square | v1: broadcast + integer sqrt with rounding fix
// Compute isqrt with sqrtl (long double), then adjust up/down — never trust
// floating point exactly at the boundary.
// Run: make run PROB=24_perfect_square SRC=result/v1_basic.cpp NP=4 ARGS="2500"

#include <cstdio>
#include <cstdlib>
#include <cmath>
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

    uint64_t r = (uint64_t)sqrtl((long double)n);
    while (r > 0 && r * r > n) --r;         // fix rounding up
    while ((r + 1) * (r + 1) <= n) ++r;     // fix rounding down

    bool sq = (r * r == n);
    if (BCL::rank() == 0)
        printf("%llu is %sa perfect square", n, sq ? "" : "NOT ");
    if (BCL::rank() == 0 && sq)
        printf(" (%llu = %llu^2)\n", n, r);
    else if (BCL::rank() == 0)
        printf("\n");

    BCL::finalize();
    return 0;
}
