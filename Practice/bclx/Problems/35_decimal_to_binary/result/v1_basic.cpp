// 35 — Decimal to Binary conversion | v1: repeated division, print reversed
// Run: make run PROB=35_decimal_to_binary SRC=result/v1_basic.cpp NP=4 ARGS="45"

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

    char buf[65];
    uint64_t k = 0;
    uint64_t x = n;
    if (x == 0) buf[k++] = '0';
    while (x > 0) { buf[k++] = (char)('0' + (x & 1)); x >>= 1; }

    if (BCL::rank() == 0) {
        printf("%llu in binary = ", n);
        for (int64_t i = (int64_t)k - 1; i >= 0; --i) putchar(buf[i]);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
