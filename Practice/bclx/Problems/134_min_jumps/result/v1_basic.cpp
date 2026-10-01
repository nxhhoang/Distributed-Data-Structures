// 134 — Minimum Number of Jumps to Reach the End of an Array | v1: greedy reach
// values are 1..5, so the end is always reachable. Greedy: extend the
// farthest reach; every time the current window ends, one more jump.
// Run: make run PROB=134_min_jumps SRC=result/v1_basic.cpp NP=4 ARGS="12"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 40503ull + 5) % 5 + 1;   // 1..5
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
        std::vector<uint64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

        if (n == 1) {
            printf("already at the end: 0 jumps\n");
        } else {
            uint64_t jumps = 0, cur_end = 0, farthest = 0;
            bool reached = false;
            for (uint64_t i = 0; i + 1 < n && !reached; ++i) {
                if (i + a[i] > farthest) farthest = i + a[i];
                if (i == cur_end) {          // must jump to continue
                    ++jumps;
                    cur_end = farthest;
                    if (cur_end + 1 >= n) reached = true;
                }
            }
            printf("minimum jumps to the end = %llu\n", jumps);
        }
    }

    BCL::finalize();
    return 0;
}
