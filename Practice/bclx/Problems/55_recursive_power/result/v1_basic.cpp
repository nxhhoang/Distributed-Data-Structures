// 55 — Power of a Number (recursion) | v1: linear recursion
// power(b, e) = b * power(b, e-1). Overflow: keep the result well below 2^64.
// Run: make run PROB=55_recursive_power SRC=result/v1_basic.cpp NP=4 ARGS="2 10"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t power(uint64_t b, uint64_t e) {
    return e == 0 ? 1 : b * power(b, e - 1);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <base> <exp>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t b = 0, e = 0;
    if (BCL::rank() == 0) {
        b = strtoull(argv[1], nullptr, 10);
        e = strtoull(argv[2], nullptr, 10);
    }
    b = BCL::broadcast(b, 0);
    e = BCL::broadcast(e, 0);

    if (BCL::rank() == 0)
        printf("%llu^%llu = %llu (linear recursion)\n", b, e, power(b, e));

    BCL::finalize();
    return 0;
}
