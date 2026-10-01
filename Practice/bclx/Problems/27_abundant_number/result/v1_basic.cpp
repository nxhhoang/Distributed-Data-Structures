// 27 — Abundant Number | v1: broadcast + proper divisor sum
// A number is abundant when the sum of its proper divisors EXCEEDS it
// (e.g. 12: 1+2+3+4+6 = 16 > 12).
// Run: make run PROB=27_abundant_number SRC=result/v1_basic.cpp NP=4 ARGS="12"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t proper_divisor_sum(uint64_t n) {
    if (n < 2) return 0;
    uint64_t s = 1;
    for (uint64_t i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            s += i;
            if (i != n / i) s += n / i;
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
    bool abundant = (s > n);
    if (BCL::rank() == 0)
        printf("%llu is %san abundant number (proper divisor sum = %llu)\n",
               n, abundant ? "" : "NOT ", s);

    BCL::finalize();
    return 0;
}
