// 162 — Find a Specific Pair in a Matrix | v1: bottom-right max table
// The GFG "specific pair": maximize mat[c][d] - mat[a][b] over all a < c AND
// b < d. Precompute maxBR[r][c] = max of the sub-matrix m[r..R)[c..C); then
// for every top-left (a, b) the best partner lies inside maxBR[a+1][b+1],
// so the whole search collapses to one lookup per cell — O(R*C).
// Run: make run PROB=162_specific_pair SRC=result/v1_basic.cpp NP=4 ARGS="4 5"

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t mat_at(uint64_t r, uint64_t c) {
    return ((r * 31 + c * 17) * 2654435761ull + 13) % 100;
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
    if (R < 2) R = 2;
    if (C < 2) C = 2;

    if (BCL::rank() == 0) {
        std::vector<std::vector<uint64_t>> m(R, std::vector<uint64_t>(C));
        for (uint64_t r = 0; r < R; ++r)
            for (uint64_t c = 0; c < C; ++c)
                m[r][c] = mat_at(r, c);

        // maxBR[r][c] = max of m[r..R)[c..C)
        std::vector<std::vector<uint64_t>> maxBR(R, std::vector<uint64_t>(C, 0));
        maxBR[R - 1][C - 1] = m[R - 1][C - 1];
        for (int64_t c = (int64_t)C - 2; c >= 0; --c)
            maxBR[R - 1][c] = std::max(m[R - 1][c], maxBR[R - 1][c + 1]);
        for (int64_t r = (int64_t)R - 2; r >= 0; --r) {
            maxBR[r][C - 1] = std::max(m[r][C - 1], maxBR[r + 1][C - 1]);
            for (int64_t c = (int64_t)C - 2; c >= 0; --c)
                maxBR[r][c] = std::max(m[r][c],
                                       std::max(maxBR[r + 1][c], maxBR[r][c + 1]));
        }

        // best pair: one lookup per (a, b)
        int64_t best = INT64_MIN;
        uint64_t ba = 0, bb = 0;
        for (uint64_t a = 0; a + 1 < R; ++a)
            for (uint64_t b = 0; b + 1 < C; ++b) {
                int64_t cand = (int64_t)maxBR[a + 1][b + 1] - (int64_t)m[a][b];
                if (cand > best) { best = cand; ba = a; bb = b; }
            }

        // one concrete partner (c, d) achieving the max, for display
        uint64_t target = maxBR[ba + 1][bb + 1], bc = ba + 1, bd = bb + 1;
        for (uint64_t c = ba + 1; c < R && m[bc][bd] != target; ++c)
            for (uint64_t d = bb + 1; d < C; ++d)
                if (m[c][d] == target) { bc = c; bd = d; break; }

        printf("best pair: mat(%llu,%llu)=%llu ... mat(%llu,%llu)=%llu -> difference %lld\n",
               ba, bb, m[ba][bb], bc, bd, m[bc][bd], best);
    }

    BCL::finalize();
    return 0;
}
