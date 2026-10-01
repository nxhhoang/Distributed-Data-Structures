// 61 — LCM of Two Numbers (recursion) | v1: lcm on top of the recursive gcd
// Run: make run PROB=61_recursive_lcm SRC=result/v1_basic.cpp NP=4 ARGS="12 18"

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

    uint64_t lcm = (a == 0 || b == 0) ? 0 : a / gcd(a, b) * b;
    if (BCL::rank() == 0)
        printf("LCM(%llu, %llu) = %llu (recursive gcd)\n", a, b, lcm);

    BCL::finalize();
    return 0;
}
