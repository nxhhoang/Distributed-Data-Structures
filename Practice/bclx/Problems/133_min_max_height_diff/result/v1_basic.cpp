// 133 — Minimize the Maximum Difference Between Heights | v1: sort + candidates
// Each height may be raised or lowered by exactly k; minimize (max - min).
// After sorting, the candidates for the extremes come from the array ends:
// small = min(a[0]+k, a[i]-k), big = max(a[i-1]+k, a[n-1]-k) for every cut i.
// Run: make run PROB=133_min_max_height_diff SRC=result/v1_basic.cpp NP=4 ARGS="10 6"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 50 + 1;   // heights 1..50
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

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);
        std::sort(a.begin(), a.end());

        uint64_t ans = a[n - 1] - a[0];   // no operation can beat doing nothing
        for (uint64_t i = 1; i < n; ++i) {
            // a[0..i-1] raised by k, a[i..n-1] lowered by k
            uint64_t small = std::min(a[0] + k, a[i] - k);
            uint64_t big = std::max(a[i - 1] + k, a[n - 1] - k);
            if (big > small && big - small < ans) ans = big - small;
        }

        printf("minimum possible (max - min) after +-%llu = %llu\n", k, ans);
    }

    BCL::finalize();
    return 0;
}
