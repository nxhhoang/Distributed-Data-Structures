// 160 — Find the Row With the Maximum Number of 1's | v1: per-row first-1 bsearch
// Rows are sorted (0s then 1s); the count for a row is C - (index of first 1).
// Run: make run PROB=160_row_max_ones SRC=result/v1_basic.cpp NP=4 ARGS="6 8"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t first_one(uint64_t r, uint64_t C) {
    return (r * 3 + 1) % C;   // column where the 1's start in row r
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <rows> <cols>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t R = 0, C = 0;
    if (BCL::rank() == 0) {
        R = strtoull(argv[1], nullptr, 10);
        C = strtoull(argv[2], nullptr, 10);
    }
    R = BCL::broadcast(R, 0);
    C = BCL::broadcast(C, 0);
    if (R == 0) R = 1;
    if (C == 0) C = 1;

    if (BCL::rank() == 0) {
        uint64_t best_row = 0, best_count = 0;
        for (uint64_t r = 0; r < R; ++r) {
            // binary search for the first 1 in row r
            uint64_t lo = 0, hi = C;
            while (lo < hi) {
                uint64_t mid = lo + (hi - lo) / 2;
                if (mid >= first_one(r, C)) hi = mid;   // 1
                else lo = mid + 1;                      // 0
            }
            uint64_t count = C - lo;
            if (count > best_count) { best_count = count; best_row = r; }
        }

        printf("row %llu has the most 1's: %llu of %llu\n", best_row, best_count, C);
    }

    BCL::finalize();
    return 0;
}
