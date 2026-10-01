// 159 — Find the Median in a Row-Wise Sorted Matrix | v1: value-range bsearch
// The median of R*C elements (odd count) is found by binary searching the
// VALUE range: count elements <= mid with an upper_bound per row, and move
// until exactly (R*C+1)/2 elements are <= the answer. O(R log C log range).
// Run: make run PROB=159_median_row_sorted SRC=result/v1_basic.cpp NP=4 ARGS="3 5"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 1000;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <rows> <cols (odd total)>\n", argv[0]);
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
    if (C % 2 == 0) C += 1;   // keep R*C odd so the median is an element

    if (BCL::rank() == 0) {
        std::vector<std::vector<uint64_t>> a(R, std::vector<uint64_t>(C));
        for (uint64_t r = 0; r < R; ++r) {
            for (uint64_t c = 0; c < C; ++c) a[r][c] = value_at(r * C + c);
            std::sort(a[r].begin(), a[r].end());   // rows sorted, columns not
        }

        uint64_t lo = ~0ull, hi = 0;
        for (uint64_t r = 0; r < R; ++r) {
            lo = std::min(lo, a[r][0]);
            hi = std::max(hi, a[r][C - 1]);
        }

        uint64_t need = (R * C + 1) / 2;   // median = smallest v with count(v) >= need
        while (lo < hi) {
            uint64_t mid = lo + (hi - lo) / 2;
            uint64_t cnt = 0;
            for (uint64_t r = 0; r < R; ++r)
                cnt += std::upper_bound(a[r].begin(), a[r].end(), mid) - a[r].begin();
            if (cnt < need) lo = mid + 1;
            else hi = mid;
        }

        printf("median of the %llu x %llu row-sorted matrix = %llu\n", R, C, lo);
    }

    BCL::finalize();
    return 0;
}
