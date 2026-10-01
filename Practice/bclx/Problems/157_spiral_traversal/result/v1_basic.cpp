// 157 — Spiral Traversal on a Matrix | v1: shrinking boundaries
// Walk the top row, right column, bottom row and left column of the current
// frame, then shrink it inward.
// Run: make run PROB=157_spiral_traversal SRC=result/v1_basic.cpp NP=4 ARGS="4 5"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t mat_at(uint64_t r, uint64_t c) {
    return ((r * 7 + c) * 2654435761ull + 17) % 100;
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
        uint64_t top = 0, bottom = R - 1, left = 0, right = C - 1;
        printf("spiral: ");
        while (top <= bottom && left <= right) {
            for (uint64_t c = left; c <= right; ++c) printf("%llu ", mat_at(top, c));
            for (uint64_t r = top + 1; r <= bottom; ++r) printf("%llu ", mat_at(r, right));
            if (top < bottom)
                for (int64_t c = (int64_t)right - 1; c >= (int64_t)left; --c)
                    printf("%llu ", mat_at(bottom, c));
            if (left < right)
                for (int64_t r = (int64_t)bottom - 1; r > (int64_t)top; --r)
                    printf("%llu ", mat_at(r, left));
            ++top;
            if (bottom == 0) break;
            --bottom;
            ++left;
            if (right == 0) break;
            --right;
        }
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
