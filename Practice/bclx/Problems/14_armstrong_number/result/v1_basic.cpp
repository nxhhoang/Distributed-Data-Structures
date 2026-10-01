// 14 — Armstrong Number | v1: broadcast, sum of digit^num_digits
// An Armstrong number equals the sum of its digits each raised to the power
// of the digit count (e.g. 153 = 1^3 + 5^3 + 3^3).
// Run: make run PROB=14_armstrong_number SRC=result/v1_basic.cpp NP=4 ARGS="153"

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

    // count digits
    uint64_t digits = 1;
    for (uint64_t x = n; x >= 10; x /= 10) ++digits;

    // sum of digit^digits
    uint64_t sum = 0;
    for (uint64_t x = n; x > 0; x /= 10) {
        uint64_t p = 1;
        for (uint64_t k = 0; k < digits; ++k) p *= x % 10;
        sum += p;
    }

    bool arm = (sum == n);
    if (BCL::rank() == 0)
        printf("%llu is %san Armstrong number (%llu digits, digit-power sum = %llu)\n",
               n, arm ? "" : "NOT ", digits, sum);

    BCL::finalize();
    return 0;
}
