// 29 — Highest Common Factor (HCF) | v1: broadcast + iterative Euclid
// Run: make run PROB=29_hcf SRC=result/v1_basic.cpp NP=4 ARGS="36 60"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

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

    uint64_t x = a, y = b;
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }

    if (BCL::rank() == 0)
        printf("HCF(%llu, %llu) = %llu\n", a, b, x);

    BCL::finalize();
    return 0;
}
