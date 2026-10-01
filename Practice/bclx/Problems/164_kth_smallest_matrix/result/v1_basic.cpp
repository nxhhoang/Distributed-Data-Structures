// 164 — Kth Smallest Element in a Row-Column Wise Sorted Matrix | v1
// Binary search over the VALUE range: count elements <= mid with an
// upper_bound per row (each row is sorted); shrink until the kth lands.
// O(R log C log range).
// Run: make run PROB=164_kth_smallest_matrix SRC=result/v1_basic.cpp NP=4 ARGS="4 5 8"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

static uint64_t mat_at(uint64_t r, uint64_t c) {
    return 10 * r + 2 * c + 1;   // strictly row-column sorted, all distinct
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "usage: %s <rows> <cols> <k>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t R = 0, C = 0, k = 0;
    if (BCL::rank() == 0) {
        R = strtoull(argv[1], nullptr, 10);
        C = strtoull(argv[2], nullptr, 10);
        k = strtoull(argv[3], nullptr, 10);
    }
    R = BCL::broadcast(R, 0);
    C = BCL::broadcast(C, 0);
    k = BCL::broadcast(k, 0);
    if (R == 0) R = 1;
    if (C == 0) C = 1;
    if (k == 0 || k > R * C) k = 1;

    if (BCL::rank() == 0) {
        std::vector<std::vector<uint64_t>> a(R, std::vector<uint64_t>(C));
        for (uint64_t r = 0; r < R; ++r)
            for (uint64_t c = 0; c < C; ++c)
                a[r][c] = mat_at(r, c);

        uint64_t lo = a[0][0], hi = a[R - 1][C - 1];
        while (lo < hi) {
            uint64_t mid = lo + (hi - lo) / 2;
            uint64_t cnt = 0;
            for (uint64_t r = 0; r < R; ++r)
                cnt += std::upper_bound(a[r].begin(), a[r].end(), mid) - a[r].begin();
            if (cnt < k) lo = mid + 1;
            else hi = mid;
        }

        printf("%llu-th smallest element = %llu (of %llu)\n", k, lo, R * C);
    }

    BCL::finalize();
    return 0;
}
