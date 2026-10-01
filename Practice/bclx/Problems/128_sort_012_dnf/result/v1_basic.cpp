// 128 — Sort an Array of 0s, 1s and 2s (no sorting algorithm) | v1: Dutch National Flag
// One pass, three regions: [0..lo) = 0s, [lo..mid) = 1s, (hi..n) = 2s.
// Run: make run PROB=128_sort_012_dnf SRC=result/v1_basic.cpp NP=4 ARGS="16"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 3;
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
        std::vector<uint64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

        uint64_t lo = 0, mid = 0, hi = n - 1;
        while (mid <= hi) {
            if (a[mid] == 0) {
                uint64_t t = a[lo]; a[lo] = a[mid]; a[mid] = t;
                ++lo; ++mid;
            } else if (a[mid] == 1) {
                ++mid;
            } else {
                uint64_t t = a[mid]; a[mid] = a[hi]; a[hi] = t;
                --hi;
            }
        }

        printf("sorted: ");
        for (uint64_t i = 0; i < n && i < 40; ++i) printf("%llu ", a[i]);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
