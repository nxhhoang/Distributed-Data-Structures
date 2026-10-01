// 89 — Finding Minimum Scalar Product of Two Vectors | v1: sort a asc, b desc, dot
// By the rearrangement inequality, pairing ascending with descending
// minimizes the scalar product.
// Run: make run PROB=89_min_scalar_product SRC=result/v1_basic.cpp NP=4 ARGS="8"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n == 0) n = 1;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n), b(n);
        for (uint64_t i = 0; i < n; ++i) {
            a[i] = value_at(i);
            b[i] = value_at(i + 7777);
        }
        std::sort(a.begin(), a.end());
        std::sort(b.begin(), b.end(), std::greater<uint64_t>());

        uint64_t dot = 0;
        for (uint64_t i = 0; i < n; ++i) dot += a[i] * b[i];
        printf("minimum scalar product = %llu\n", dot);
    }

    BCL::finalize();
    return 0;
}
