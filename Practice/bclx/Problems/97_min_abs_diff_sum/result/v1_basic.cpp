// 97 — Finding the Minimum Sum of Absolute Difference | v1: sort + pair adjacent
// Pair the sorted elements two by two: (a0,a1), (a2,a3), ... — the pairing
// that minimizes the total absolute difference.
// Run: make run PROB=97_min_abs_diff_sum SRC=result/v1_basic.cpp NP=4 ARGS="10"

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
        fprintf(stderr, "usage: %s <n (pairs of it)>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n < 2) n = 2;
    if (n % 2 == 1) n -= 1;   // pair up evenly

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);
        std::sort(a.begin(), a.end());

        uint64_t sum = 0;
        for (uint64_t i = 0; i + 1 < n; i += 2)
            sum += a[i + 1] - a[i];   // sorted, so non-negative

        printf("minimum sum of absolute differences = %llu\n", sum);
    }

    BCL::finalize();
    return 0;
}
