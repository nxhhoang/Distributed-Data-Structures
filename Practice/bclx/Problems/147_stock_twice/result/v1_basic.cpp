// 147 — Maximum Profit by Buying and Selling a Share At Most Twice | v1
// Two passes: profit_left[i] = best single transaction within days [0..i];
// profit_right[i] = best within [i..n). The answer is the best split point.
// Run: make run PROB=147_stock_twice SRC=result/v1_basic.cpp NP=4 ARGS="12"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100 + 10;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n days>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n < 2) n = 2;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

        // best single transaction ending at or before day i
        std::vector<uint64_t> left(n);
        uint64_t mn = a[0];
        left[0] = 0;
        for (uint64_t i = 1; i < n; ++i) {
            left[i] = left[i - 1];
            if (a[i] > mn && a[i] - mn > left[i]) {   // guard: a[i] - mn would underflow uint64
                left[i] = a[i] - mn;
            }
            if (a[i] < mn) mn = a[i];
        }

        // best single transaction starting at or after day i
        std::vector<uint64_t> right(n);
        uint64_t mx = a[n - 1];
        right[n - 1] = 0;
        for (int64_t i = (int64_t)n - 2; i >= 0; --i) {
            right[i] = std::max(right[i + 1], mx - a[i]);
            if (a[i] > mx) mx = a[i];
        }

        uint64_t best = right[0];   // all days in one transaction
        for (uint64_t i = 0; i + 1 < n; ++i)
            best = std::max(best, left[i] + right[i + 1]);

        printf("maximum profit with at most two transactions = %llu\n", best);
    }

    BCL::finalize();
    return 0;
}
