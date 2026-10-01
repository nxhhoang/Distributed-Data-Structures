// 138 — Merge Intervals | v1: sort by start + sweep
// Intervals overlap when next.start <= current.end — merge them into one.
// Run: make run PROB=138_merge_intervals SRC=result/v1_basic.cpp NP=4 ARGS="8"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

struct interval {
    uint64_t s, e;
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n intervals>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n == 0) n = 1;

    if (BCL::rank() == 0) {
        std::vector<interval> v(n);
        for (uint64_t i = 0; i < n; ++i) {
            v[i].s = 5 * i;                                  // sorted starts
            v[i].e = v[i].s + 3 + (i % 4) * 2;               // varying overlap
        }
        std::sort(v.begin(), v.end(),
                  [](const interval &a, const interval &b) { return a.s < b.s; });

        std::vector<interval> out;
        out.push_back(v[0]);
        for (uint64_t i = 1; i < n; ++i) {
            if (v[i].s <= out.back().e) {
                if (v[i].e > out.back().e) out.back().e = v[i].e;
            } else {
                out.push_back(v[i]);
            }
        }

        printf("%zu intervals in -> %zu merged: ", n, out.size());
        for (const interval &iv : out) printf("[%llu, %llu] ", iv.s, iv.e);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
