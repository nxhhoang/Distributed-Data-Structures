// 136 — Merge 2 Sorted Arrays Without Using Extra Space | v1: gap method
// Treat the concatenation as one array and shell-sort it with shrinking gaps
// (gap = ceil(total/2), then ceil(gap/2), ... 1) — each pass only swaps
// out-of-order pairs, converging to a fully sorted merge in place.
// Run: make run PROB=136_merge_sorted_inplace SRC=result/v1_basic.cpp NP=4 ARGS="6 7"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_a(uint64_t i) { return 2 * i + 1; }       // odd
static uint64_t value_b(uint64_t i) { return 2 * i + 2; }       // even... plus offset

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
        for (uint64_t i = 0; i < n; ++i) a[i] = value_a(i);   // sorted
        for (uint64_t i = 0; i < m; ++i) b[i] = value_b(i);   // sorted

        auto get = [&](uint64_t k) -> uint64_t & {
            return k < n ? a[k] : b[k - n];
        };

        uint64_t total = n + m;
        for (uint64_t gap = (total + 1) / 2; gap > 0; gap = (gap == 1) ? 0 : (gap + 1) / 2) {
            for (uint64_t i = 0; i + gap < total; ++i) {
                uint64_t j = i + gap;
                if (get(i) > get(j)) {
                    uint64_t t = get(i);
                    get(i) = get(j);
                    get(j) = t;
                }
            }
        }

        printf("merged A: ");
        for (uint64_t i = 0; i < n; ++i) printf("%llu ", a[i]);
        printf("\nmerged B: ");
        for (uint64_t i = 0; i < m; ++i) printf("%llu ", b[i]);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
