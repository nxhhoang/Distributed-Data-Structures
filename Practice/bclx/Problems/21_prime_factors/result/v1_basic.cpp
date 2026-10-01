// 21 — Finding Prime Factors of a Number | v1: trial division on rank 0
// Classic loop: strip 2s, then odd candidates up to sqrt(n).
// Run: make run PROB=21_prime_factors SRC=result/v1_basic.cpp NP=4 ARGS="360"

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

    if (BCL::rank() == 0) {
        printf("%llu = ", n);
        uint64_t first = 1;
        uint64_t x = n;
        while (x % 2 == 0) {
            printf(first ? "2" : " * 2"); first = 0; x /= 2;
        }
        for (uint64_t d = 3; d * d <= x; d += 2) {
            while (x % d == 0) {
                printf(first ? "%llu" : " * %llu", d); first = 0; x /= d;
            }
        }
        if (x > 1) printf(first ? "%llu" : " * %llu", x);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
