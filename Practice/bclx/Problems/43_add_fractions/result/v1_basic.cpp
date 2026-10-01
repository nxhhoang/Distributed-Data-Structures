// 43 — Addition of Two Fractions | v1: broadcast 4 values, common denominator
// a/b + c/d: denominator = lcm(b, d), then reduce with gcd.
// Run: make run PROB=43_add_fractions SRC=result/v1_basic.cpp NP=4 ARGS="1 2 3 4"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t gcd(uint64_t a, uint64_t b) {
    while (b != 0) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "usage: %s <a> <b> <c> <d>   (computes a/b + c/d)\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t a = 0, b = 0, c = 0, d = 0;
    if (BCL::rank() == 0) {
        a = strtoull(argv[1], nullptr, 10);
        b = strtoull(argv[2], nullptr, 10);
        c = strtoull(argv[3], nullptr, 10);
        d = strtoull(argv[4], nullptr, 10);
    }
    a = BCL::broadcast(a, 0);
    b = BCL::broadcast(b, 0);
    c = BCL::broadcast(c, 0);
    d = BCL::broadcast(d, 0);

    if (BCL::rank() == 0) {
        if (b == 0 || d == 0) {
            fprintf(stderr, "denominators must be non-zero\n");
        } else {
            uint64_t g = gcd(b, d);
            uint64_t den = b / g * d;                    // lcm of the denominators
            uint64_t num = a * (den / b) + c * (den / d); // common-denominator sum
            uint64_t g2 = gcd(num, den);                 // reduce
            num /= g2;
            den /= g2;
            printf("%llu/%llu + %llu/%llu = %llu/%llu\n", a, b, c, d, num, den);
        }
    }

    BCL::finalize();
    return 0;
}
