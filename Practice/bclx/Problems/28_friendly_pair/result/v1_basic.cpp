// 28 — Friendly Pair | v1: broadcast + abundancy index comparison
// Two numbers are "friendly" when their abundancy indices sigma(n)/n are
// equal, where sigma is the sum of ALL divisors (e.g. 30 and 140 both have
// sigma/n = 12/5). Compared by cross-multiplication in unsigned __int128
// to stay exact.
// Run: make run PROB=28_friendly_pair SRC=result/v1_basic.cpp NP=4 ARGS="30 140"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t divisor_sum(uint64_t n) {   // ALL divisors, n included
    if (n == 0) return 0;
    uint64_t s = 0;
    for (uint64_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            s += i;
            if (i != n / i) s += n / i;
        }
    }
    return s;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <a> <b>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t a = 0, b = 0;
    if (BCL::rank() == 0) {
        a = strtoull(argv[1], nullptr, 10);
        b = strtoull(argv[2], nullptr, 10);
    }
    a = BCL::broadcast(a, 0);
    b = BCL::broadcast(b, 0);

    uint64_t sa = divisor_sum(a), sb = divisor_sum(b);
    // sigma(a)/a == sigma(b)/b  <=>  sigma(a)*b == sigma(b)*a  (exact)
    unsigned __int128 lhs = (unsigned __int128)sa * b;
    unsigned __int128 rhs = (unsigned __int128)sb * a;
    bool friendly = (a > 0 && b > 0 && lhs == rhs);

    if (BCL::rank() == 0)
        printf("(%llu, %llu) is %sa friendly pair (sigma = %llu, %llu)\n",
               a, b, friendly ? "" : "NOT", sa, sb);

    BCL::finalize();
    return 0;
}
