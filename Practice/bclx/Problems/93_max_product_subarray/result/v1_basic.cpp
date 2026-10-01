// 93 — Find the Maximum Product Sub-array | v1: Kadane with min/max tracking
// Track both the maximum and minimum products ending at each position
// (a negative element swaps them); handles zeros naturally.
// Run: make run PROB=93_max_product_subarray SRC=result/v1_basic.cpp NP=4 ARGS="12"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

// values in [-9, 9] so negatives and zeros occur
static int64_t value_at(uint64_t i) {
    return (int64_t)((i * 2654435761ull + 17) % 19) - 9;
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
        std::vector<int64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

        int64_t best = a[0], cur_max = a[0], cur_min = a[0];
        for (uint64_t i = 1; i < n; ++i) {
            if (a[i] < 0) std::swap(cur_max, cur_min);
            cur_max = std::max(a[i], cur_max * a[i]);
            cur_min = std::min(a[i], cur_min * a[i]);
            best = std::max(best, cur_max);
        }

        printf("array: ");
        for (uint64_t i = 0; i < n && i < 20; ++i) printf("%lld ", a[i]);
        printf("\nmaximum product sub-array = %lld\n", best);
    }

    BCL::finalize();
    return 0;
}
