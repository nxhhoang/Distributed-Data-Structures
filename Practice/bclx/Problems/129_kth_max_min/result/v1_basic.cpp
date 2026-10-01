// 129 — Find the Kth Max and Min Element of an Array | v1: sort + index
// k-th min = sorted[k-1]; k-th max = sorted[n-k] (distinct ranks assume
// distinct values; duplicates share a position here).
// Run: make run PROB=129_kth_max_min SRC=result/v1_basic.cpp NP=4 ARGS="16 3"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 1000;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n> <k>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0, k = 0;
    if (BCL::rank() == 0) {
        n = strtoull(argv[1], nullptr, 10);
        k = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    k = BCL::broadcast(k, 0);
    if (n == 0) n = 1;
    if (k == 0 || k > n) k = 1;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);
        std::sort(a.begin(), a.end());

        printf("%llu-th smallest = %llu, %llu-th largest = %llu\n",
               k, a[k - 1], k, a[n - k]);
    }

    BCL::finalize();
    return 0;
}
