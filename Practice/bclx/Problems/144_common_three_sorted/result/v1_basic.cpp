// 144 — Find Common Elements in 3 Sorted Arrays | v1: three-pointer walk
// A = multiples of 6, B = multiples of 4, C = multiples of 3 — the common
// elements are exactly the multiples of 12.
// Run: make run PROB=144_common_three_sorted SRC=result/v1_basic.cpp NP=4 ARGS="10 12 15"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_a(uint64_t i) { return 6 * i; }
static uint64_t value_b(uint64_t i) { return 4 * i; }
static uint64_t value_c(uint64_t i) { return 3 * i; }

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "usage: %s <n A> <m B> <k C>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0, m = 0, k = 0;
    if (BCL::rank() == 0) {
        n = strtoull(argv[1], nullptr, 10);
        m = strtoull(argv[2], nullptr, 10);
        k = strtoull(argv[3], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    m = BCL::broadcast(m, 0);
    k = BCL::broadcast(k, 0);
    if (n == 0) n = 1;
    if (m == 0) m = 1;
    if (k == 0) k = 1;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n), b(m), c(k);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_a(i);
        for (uint64_t i = 0; i < m; ++i) b[i] = value_b(i);
        for (uint64_t i = 0; i < k; ++i) c[i] = value_c(i);

        std::vector<uint64_t> common;
        uint64_t i = 0, j = 0, l = 0;
        while (i < n && j < m && l < k) {
            if (a[i] == b[j] && b[j] == c[l]) {
                if (common.empty() || common.back() != a[i]) common.push_back(a[i]);
                ++i; ++j; ++l;
            } else if (a[i] <= b[j] && a[i] <= c[l]) {
                ++i;
            } else if (b[j] <= a[i] && b[j] <= c[l]) {
                ++j;
            } else {
                ++l;
            }
        }

        printf("common elements: ");
        for (uint64_t v : common) printf("%llu ", v);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
