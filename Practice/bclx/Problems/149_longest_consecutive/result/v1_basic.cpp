// 149 — Find the Longest Consecutive Subsequence | v1: set + run-start check
// Only values v with v-1 absent can START a run, so every element is visited
// at most twice — O(n) overall.
// Run: make run PROB=149_longest_consecutive SRC=result/v1_basic.cpp NP=4 ARGS="30"

#include <cstdio>
#include <cstdlib>
#include <set>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
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
        std::set<uint64_t> s;
        for (uint64_t i = 0; i < n; ++i) s.insert(value_at(i));

        uint64_t best = 0, best_start = 0;
        for (uint64_t v : s) {
            if (s.count(v - 1)) continue;   // not a run start
            uint64_t len = 1;
            while (s.count(v + len)) ++len;
            if (len > best) { best = len; best_start = v; }
        }

        printf("longest consecutive run: %llu (starting at %llu)\n", best, best_start);
    }

    BCL::finalize();
    return 0;
}
