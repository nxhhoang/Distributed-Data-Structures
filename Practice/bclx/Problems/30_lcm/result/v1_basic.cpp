// 30 — Lowest Common Multiple (LCM) | v1: broadcast + lcm via HCF
// lcm(a, b) = a / hcf(a,b) * b — divide FIRST to avoid overflow.
// Run: make run PROB=30_lcm SRC=result/v1_basic.cpp NP=4 ARGS="12 18"

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

    uint64_t hcf = a, y = b;
    while (y != 0) { uint64_t t = hcf % y; hcf = y; y = t; }
    uint64_t lcm = (a == 0 || b == 0) ? 0 : a / hcf * b;

    if (BCL::rank() == 0)
        printf("LCM(%llu, %llu) = %llu\n", a, b, lcm);

    BCL::finalize();
    return 0;
}
