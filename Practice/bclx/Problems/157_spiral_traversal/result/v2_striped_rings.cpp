// 157 — Spiral Traversal | v2: stripe the RINGS over ranks
// A spiral is a sequence of concentric, INDEPENDENT rings — ring k is bounded
// by rows/cols [k, n-1-k]. Rank me prints the rings k with k % P == me
// (each ring's own order is the spiral order). An allreduce count check
// verifies that the ranks together covered all R*C cells.
// Run: make run PROB=157_spiral_traversal SRC=result/v2_striped_rings.cpp NP=4 ARGS="6 6"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t mat_at(uint64_t r, uint64_t c) {
    return ((r * 7 + c) * 2654435761ull + 17) % 100;
}

// print one full ring; returns how many cells it contains
static uint64_t ring_print(uint64_t k, uint64_t R, uint64_t C, uint64_t me) {
    uint64_t top = k, bottom = R - 1 - k, left = k, right = C - 1 - k;
    if (top > bottom || left > right) return 0;

    uint64_t cnt = 0;
    printf("[rank %llu] ring %llu: ", me, k);

    if (top == bottom) {                       // degenerated to a single row
        for (uint64_t c = left; c <= right; ++c) { printf("%llu ", mat_at(top, c)); ++cnt; }
    } else if (left == right) {                // single column
        for (uint64_t r = top; r <= bottom; ++r) { printf("%llu ", mat_at(r, left)); ++cnt; }
    } else {
        for (uint64_t c = left; c <= right; ++c) { printf("%llu ", mat_at(top, c)); ++cnt; }
        for (uint64_t r = top + 1; r <= bottom; ++r) { printf("%llu ", mat_at(r, right)); ++cnt; }
        for (int64_t c = (int64_t)right - 1; c >= (int64_t)left; --c) {
            printf("%llu ", mat_at(bottom, c)); ++cnt;
        }
        for (int64_t r = (int64_t)bottom - 1; r > (int64_t)top; --r) {
            printf("%llu ", mat_at(r, left)); ++cnt;
        }
    }
    printf("\n");
    return cnt;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <rows> <cols>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t R = 0, C = 0;
    if (me == 0) {
        R = strtoull(argv[1], nullptr, 10);
        C = strtoull(argv[2], nullptr, 10);
    }
    R = BCL::broadcast(R, 0);
    C = BCL::broadcast(C, 0);
    if (R == 0) R = 1;
    if (C == 0) C = 1;

    // number of concentric rings = ceil(min(R, C) / 2)
    uint64_t mn = (R < C) ? R : C;
    uint64_t rings = (mn + 1) / 2;

    uint64_t count = 0;
    for (uint64_t k = me; k < rings; k += P)
        count += ring_print(k, R, C, me);

    uint64_t total = bclx::allreduce(count, BCL::sum<uint64_t>{});
    if (me == 0)
        printf("rings covered %llu cells (expected %llu -> %s)\n",
               total, R * C, total == R * C ? "MATCH" : "MISMATCH!");

    BCL::finalize();
    return 0;
}
