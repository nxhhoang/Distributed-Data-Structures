// 26 — Harshad Number | v1: broadcast + divisibility by digit sum
// A Harshad (Niven) number is divisible by the sum of its digits
// (e.g. 18 -> 1+8 = 9, and 18 % 9 == 0).
// Run: make run PROB=26_harshad_number SRC=result/v1_basic.cpp NP=4 ARGS="18"

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

    uint64_t sum = 0;
    for (uint64_t x = n; x > 0; x /= 10) sum += x % 10;

    bool harshad = (n > 0 && sum > 0 && n % sum == 0);
    if (BCL::rank() == 0)
        printf("%llu is %sa Harshad number (digit sum = %llu)\n",
               n, harshad ? "" : "NOT ", sum);

    BCL::finalize();
    return 0;
}
