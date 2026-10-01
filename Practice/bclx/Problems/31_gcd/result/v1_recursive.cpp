// 31 — Greatest Common Divisor (GCD) | v1: recursive Euclid
// Same quantity as HCF (problem 29) — shown here with the recursive form,
// the classic gcd(a, b) = gcd(b, a mod b).
// Run: make run PROB=31_gcd SRC=result/v1_recursive.cpp NP=4 ARGS="48 180"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t gcd(uint64_t a, uint64_t b) {
    return b == 0 ? a : gcd(b, a % b);
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

    uint64_t g = gcd(a, b);
    if (BCL::rank() == 0)
        printf("GCD(%llu, %llu) = %llu\n", a, b, g);

    BCL::finalize();
    return 0;
}
