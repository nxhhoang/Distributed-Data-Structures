// 11 — Sum of Digits of a Number | v1: broadcast + digit loop
// Run: make run PROB=11_sum_of_digits SRC=result/v1_basic.cpp NP=4 ARGS="123456789"

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

    if (BCL::rank() == 0)
        printf("digit sum of %llu = %llu\n", n, n == 0 ? 0 : sum);

    BCL::finalize();
    return 0;
}
