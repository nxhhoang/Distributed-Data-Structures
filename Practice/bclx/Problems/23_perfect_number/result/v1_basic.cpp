// 23 — Perfect Number | v1: broadcast + paired-divisor sum
// A perfect number equals the sum of its proper divisors (e.g. 28 = 1+2+4+7+14).
// The divisor sum uses the paired loop (i*i <= n) — O(sqrt(n)) instead of O(n).
// Run: make run PROB=23_perfect_number SRC=result/v1_basic.cpp NP=4 ARGS="28"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t proper_divisor_sum(uint64_t n) {
    if (n < 2) return 0;
    uint64_t s = 1;                       // 1 is always a proper divisor
    for (uint64_t i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            s += i;
            if (i != n / i) s += n / i;   // add the paired divisor once
        }
    }
    return s;
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

    uint64_t s = proper_divisor_sum(n);
    bool perfect = (s == n);
    if (BCL::rank() == 0)
        printf("%llu is %sa perfect number (proper divisor sum = %llu)\n",
               n, perfect ? "" : "NOT ", s);

    BCL::finalize();
    return 0;
}
