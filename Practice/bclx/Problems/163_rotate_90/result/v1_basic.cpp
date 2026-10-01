// 163 — Rotate a Matrix by 90 Degrees | v1: transpose + reverse rows
// Clockwise 90: element (r, c) moves to (c, C-1-r). In place: transpose
// (swap (r,c) with (c,r)) then reverse every row.
// Run: make run PROB=163_rotate_90 SRC=result/v1_basic.cpp NP=4 ARGS="4"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

static uint64_t mat_at(uint64_t r, uint64_t c) {
    return r * 4 + c;   // identity-ish values for easy eyeball verification
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n (square)>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n == 0) n = 1;
    if (n > 12) n = 12;

    if (BCL::rank() == 0) {
        std::vector<std::vector<uint64_t>> a(n, std::vector<uint64_t>(n));
        for (uint64_t r = 0; r < n; ++r)
            for (uint64_t c = 0; c < n; ++c)
                a[r][c] = mat_at(r, c);

        // transpose
        for (uint64_t r = 0; r < n; ++r)
            for (uint64_t c = r + 1; c < n; ++c)
                std::swap(a[r][c], a[c][r]);
        // reverse each row
        for (uint64_t r = 0; r < n; ++r)
            std::reverse(a[r].begin(), a[r].end());

        printf("rotated 90 degrees clockwise:\n");
        for (uint64_t r = 0; r < n; ++r) {
            for (uint64_t c = 0; c < n; ++c)
                printf("%2llu ", a[r][c]);
            printf("\n");
        }
    }

    BCL::finalize();
    return 0;
}
