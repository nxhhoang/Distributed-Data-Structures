// 158 — Search an Element in a Matrix | v1: staircase walk
// In a row-column sorted matrix, start at the top-right corner: the current
// value is the largest of its row and smallest of its column, so each
// comparison eliminates a whole row or column — O(R + C).
// Run: make run PROB=158_search_matrix SRC=result/v1_basic.cpp NP=4 ARGS="4 5 21"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t mat_at(uint64_t r, uint64_t c) {
    return 10 * r + 2 * c + 1;   // strictly increasing along rows and columns
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "usage: %s <rows> <cols> <target>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t R = 0, C = 0, target = 0;
    if (BCL::rank() == 0) {
        R = strtoull(argv[1], nullptr, 10);
        C = strtoull(argv[2], nullptr, 10);
        target = strtoull(argv[3], nullptr, 10);
    }
    R = BCL::broadcast(R, 0);
    C = BCL::broadcast(C, 0);
    target = BCL::broadcast(target, 0);
    if (R == 0) R = 1;
    if (C == 0) C = 1;

    if (BCL::rank() == 0) {
        int64_t r = 0, c = (int64_t)C - 1;
        bool found = false;
        while (r < (int64_t)R && c >= 0) {
            uint64_t v = mat_at(r, c);
            if (v == target) { found = true; break; }
            if (v < target) ++r;
            else --c;
        }

        if (found)
            printf("%llu found at row %lld, col %lld\n", target, r, c);
        else
            printf("%llu not in the matrix\n", target);
    }

    BCL::finalize();
    return 0;
}
