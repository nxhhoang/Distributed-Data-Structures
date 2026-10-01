// 155 — Median of Two Sorted Arrays of Equal Size | v1: partition binary search
// Find i elements taken from A's left (n-i from B's left) so that every left
// element <= every right element; the median is the average of the boundary
// max/min. O(log n).
// Run: make run PROB=155_median_two_equal SRC=result/v1_basic.cpp NP=4 ARGS="6 6"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

static uint64_t value_a(uint64_t i) { return 2 * i + 1; }       // odds
static uint64_t value_b(uint64_t i) { return 4 * i + 2; }       // 2, 6, 10...

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n A> <m B (== n)>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0, m = 0;
    if (BCL::rank() == 0) {
        n = strtoull(argv[1], nullptr, 10);
        m = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    m = BCL::broadcast(m, 0);
    if (n != m) m = n;   // this variant needs equal sizes
    if (n == 0) n = m = 1;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n), b(m);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_a(i);
        for (uint64_t i = 0; i < m; ++i) b[i] = value_b(i);

        const uint64_t INF = ~0ull;
        uint64_t lo = 0, hi = n;
        double median = 0;
        while (lo <= hi) {
            uint64_t i = lo + (hi - lo) / 2;
            uint64_t j = n - i;

            uint64_t maxLA = (i == 0) ? 0 : a[i - 1];
            uint64_t minRA = (i == n) ? INF : a[i];
            uint64_t maxLB = (j == 0) ? 0 : b[j - 1];
            uint64_t minRB = (j == m) ? INF : b[j];

            if (maxLA <= minRB && maxLB <= minRA) {
                uint64_t l = std::max(maxLA, maxLB);
                uint64_t r = std::min(minRA, minRB);
                median = (double)(l + r) / 2.0;
                break;
            }
            if (maxLA > minRB) hi = i - 1;
            else lo = i + 1;
        }

        printf("median of the two sorted arrays = %.1f\n", median);
    }

    BCL::finalize();
    return 0;
}
