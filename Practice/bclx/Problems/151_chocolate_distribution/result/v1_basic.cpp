// 151 — Chocolate Distribution Problem | v1: sort + window of m
// Give m students one packet each from n packets; minimize the difference
// between the most and least chocolate — the best m packets are consecutive
// in the sorted order.
// Run: make run PROB=151_chocolate_distribution SRC=result/v1_basic.cpp NP=4 ARGS="12 4"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 50 + 1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n packets> <m students>\n", argv[0]);
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
    if (m == 0 || m > n) m = 1;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);
        std::sort(a.begin(), a.end());

        uint64_t best = ~0ull, best_i = 0;
        for (uint64_t i = 0; i + m <= n; ++i) {
            uint64_t diff = a[i + m - 1] - a[i];
            if (diff < best) { best = diff; best_i = i; }
        }

        printf("packets for %llu students: ", m);
        for (uint64_t i = best_i; i < best_i + m; ++i) printf("%llu ", a[i]);
        printf("(min difference = %llu)\n", best);
    }

    BCL::finalize();
    return 0;
}
