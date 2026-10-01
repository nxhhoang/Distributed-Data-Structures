// 53 — Finding the Number of Integers Which Have Exactly x Divisors | v1: rank 0
// Counts i in [1..n] whose divisor count (paired loop, O(sqrt(i))) equals x.
// Run: make run PROB=53_exactly_x_divisors SRC=result/v1_basic.cpp NP=4 ARGS="20 4"
// (numbers <= 20 with exactly 4 divisors: 6, 8, 10, 14, 15 -> 5)

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t divisor_count(uint64_t n) {
    uint64_t c = 0;
    for (uint64_t i = 1; i * i <= n; ++i)
        if (n % i == 0) c += (i == n / i) ? 1 : 2;
    return c;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n> <x>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0, x = 0;
    if (BCL::rank() == 0) {
        n = strtoull(argv[1], nullptr, 10);
        x = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    x = BCL::broadcast(x, 0);

    if (BCL::rank() == 0) {
        uint64_t count = 0;
        for (uint64_t i = 1; i <= n; ++i)
            if (divisor_count(i) == x) ++count;
        printf("numbers in [1..%llu] with exactly %llu divisors: %llu\n", n, x, count);
    }

    BCL::finalize();
    return 0;
}
