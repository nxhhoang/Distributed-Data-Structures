// 156 — Median of Two Sorted Arrays of Different Sizes | v1: generalized search
// Binary-search the partition over the SMALLER array so the other side's
// index j = (n+m+1)/2 - i always lands in range; odd totals take the max of
// the left parts, even totals average left max and right min. O(log min(n,m)).
// Run: make run PROB=156_median_two_unequal SRC=result/v1_basic.cpp NP=4 ARGS="5 9"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

static uint64_t value_a(uint64_t i) { return 3 * i + 1; }       // 1, 4, 7...
static uint64_t value_b(uint64_t i) { return 5 * i + 2; }       // 2, 7, 12...

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n A> <m B>\n", argv[0]);
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
    if (n == 0) n = 1;
    if (m == 0) m = 1;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n), b(m);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_a(i);
        for (uint64_t i = 0; i < m; ++i) b[i] = value_b(i);

        // A must be the smaller array
        if (a.size() > b.size()) std::swap(a, b);
        uint64_t s1 = a.size(), s2 = b.size();
        uint64_t total = s1 + s2, half = (total + 1) / 2;

        const uint64_t INF = ~0ull;
        uint64_t lo = 0, hi = s1;
        double median = 0;
        while (lo <= hi) {
            uint64_t i = lo + (hi - lo) / 2;
            uint64_t j = half - i;

            uint64_t maxLA = (i == 0) ? 0 : a[i - 1];
            uint64_t minRA = (i == s1) ? INF : a[i];
            uint64_t maxLB = (j == 0) ? 0 : b[j - 1];
            uint64_t minRB = (j == s2) ? INF : b[j];

            if (maxLA <= minRB && maxLB <= minRA) {
                uint64_t l = std::max(maxLA, maxLB);
                if (total % 2 == 1)
                    median = (double)l;
                else {
                    uint64_t r = std::min(minRA, minRB);
                    median = (double)(l + r) / 2.0;
                }
                break;
            }
            if (maxLA > minRB) hi = i - 1;
            else lo = i + 1;
        }

        printf("median of arrays (%llu + %llu elements) = %.1f\n",
               (unsigned long long)s1, (unsigned long long)s2, median);
    }

    BCL::finalize();
    return 0;
}
