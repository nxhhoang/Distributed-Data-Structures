// 137 — Kadane's Algorithm | v1: O(n) maximum subarray
// best_ending_here = max(a[i], best_ending_here + a[i]) — either extend the
// previous subarray or restart at a[i]. Compare with 132's O(n^2).
// Run: make run PROB=137_kadane SRC=result/v1_basic.cpp NP=4 ARGS="12"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

static int64_t value_signed(uint64_t i) {
    return (int64_t)((i * 2654435761ull + 17) % 200) - 100;
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
        for (uint64_t i = 0; i < n; ++i) a[i] = value_signed(i);

        int64_t best = a[0], cur = a[0];
        uint64_t best_s = 0, best_e = 0, s = 0;
        for (uint64_t i = 1; i < n; ++i) {
            if (a[i] > cur + a[i]) { cur = a[i]; s = i; }   // restart
            else cur += a[i];                               // extend
            if (cur > best) { best = cur; best_s = s; best_e = i; }
        }

        printf("Kadane: max subarray sum = %lld over [%llu..%llu]\n",
               best, best_s, best_e);
    }

    BCL::finalize();
    return 0;
}
