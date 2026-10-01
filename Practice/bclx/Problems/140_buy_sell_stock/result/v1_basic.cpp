// 140 — Best Time to Buy and Sell Stock (one transaction) | v1: running minimum
// The best profit at day i is a[i] minus the minimum price seen before i.
// Run: make run PROB=140_buy_sell_stock SRC=result/v1_basic.cpp NP=4 ARGS="12"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100 + 10;   // prices 10..109
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

        uint64_t best = 0, mn = a[0];
        uint64_t buy = 0, sell = 0, mn_day = 0;
        for (uint64_t i = 1; i < n; ++i) {
            if (a[i] > mn && a[i] - mn > best) {   // guard: a[i] - mn would underflow uint64
                best = a[i] - mn;
                buy = mn_day; sell = i;
            }
            if (a[i] < mn) { mn = a[i]; mn_day = i; }
        }

        printf("best profit = %llu (buy day %llu at %llu, sell day %llu at %llu)\n",
               best, buy, a[buy], sell, a[sell]);
    }

    BCL::finalize();
    return 0;
}
