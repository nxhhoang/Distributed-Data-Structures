// 25 — Automorphic Number | v1: broadcast + 128-bit square check
// n is automorphic when n*n ends with the digits of n (e.g. 76^2 = 5776).
// The square is computed in unsigned __int128 to survive n close to 2^64.
// Run: make run PROB=25_automorphic_number SRC=result/v1_basic.cpp NP=4 ARGS="76"

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

    unsigned __int128 sq = (unsigned __int128)n * n;

    // p10 = 10^(number of digits of n)
    uint64_t p10 = 1;
    for (uint64_t x = n; x >= 10; x /= 10) p10 *= 10;
    p10 *= 10;
    if (n == 0) p10 = 10;

    bool auto_morphic = ((uint64_t)(sq % p10) == n);
    if (BCL::rank() == 0)
        printf("%llu is %sautomorphic (%llu^2 ends with %s)\n",
               n, auto_morphic ? "" : "NOT ", n, auto_morphic ? "itself" : "something else");

    BCL::finalize();
    return 0;
}
