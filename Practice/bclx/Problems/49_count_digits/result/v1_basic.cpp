// 49 — Calculate the Number of Digits in an Integer | v1: broadcast + count
// Run: make run PROB=49_count_digits SRC=result/v1_basic.cpp NP=4 ARGS="123456"

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

    uint64_t digits = 1;
    for (uint64_t x = n; x >= 10; x /= 10) ++digits;

    if (BCL::rank() == 0)
        printf("%llu has %llu digit%s\n", n, digits, digits > 1 ? "s" : "");

    BCL::finalize();
    return 0;
}
