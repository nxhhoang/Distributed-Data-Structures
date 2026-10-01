// 60 — HCF of Two Numbers Using Recursion | v1: recursive Euclid
// The same recursive form already seen in 31/v1_recursive (HCF == GCD).
// Run: make run PROB=60_recursive_hcf SRC=result/v1_basic.cpp NP=4 ARGS="36 60"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t hcf(uint64_t a, uint64_t b) {
    return b == 0 ? a : hcf(b, a % b);
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

    if (BCL::rank() == 0)
        printf("HCF(%llu, %llu) = %llu (recursive Euclid)\n", a, b, hcf(a, b));

    BCL::finalize();
    return 0;
}
