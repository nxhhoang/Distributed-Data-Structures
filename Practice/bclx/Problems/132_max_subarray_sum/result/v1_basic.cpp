// 132 — Find the Largest Sum Contiguous Subarray | v1: O(n^2) all subarrays
// The brute-force view: fix a start, extend the end while keeping a running
// sum. Compare with 137 (Kadane) — same answer, O(n) instead.
// Run: make run PROB=132_max_subarray_sum SRC=result/v1_basic.cpp NP=4 ARGS="12"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static int64_t value_signed(uint64_t i) {
    return (int64_t)((i * 2654435761ull + 17) % 200) - 100;   // -100..99
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
    if (n > 3000) n = 3000;

    if (BCL::rank() == 0) {
        std::vector<int64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_signed(i);

        int64_t best = a[0];
        for (uint64_t s = 0; s < n; ++s) {
            int64_t sum = 0;
            for (uint64_t e = s; e < n; ++e) {
                sum += a[e];
                if (sum > best) best = sum;
            }
        }

        printf("largest contiguous sum (O(n^2)) = %lld\n", best);
    }

    BCL::finalize();
    return 0;
}
