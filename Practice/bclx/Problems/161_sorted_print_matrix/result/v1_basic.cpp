// 161 — Print Elements in Sorted Order Using a Row-Column Sorted Matrix | v1
// The rows are already sorted lists — a k-way merge walks the row heads and
// repeatedly emits the smallest. O(R * R * C) with a linear head scan
// (R small); a heap would make it O(R*C log R).
// Run: make run PROB=161_sorted_print_matrix SRC=result/v1_basic.cpp NP=4 ARGS="4 5"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t mat_at(uint64_t r, uint64_t c) {
    return 10 * r + 2 * c + 1;   // row-column sorted
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
    if (R > 50) R = 50;   // keep the O(R) head scan cheap

    if (BCL::rank() == 0) {
        std::vector<uint64_t> head(R, 0);   // current column per row

        printf("sorted: ");
        for (uint64_t step = 0; step < R * C; ++step) {
            uint64_t best_r = R, best_v = ~0ull;
            for (uint64_t r = 0; r < R; ++r) {
                if (head[r] < C) {
                    uint64_t v = mat_at(r, head[r]);
                    if (best_r == R || v < best_v) { best_v = v; best_r = r; }
                }
            }
            printf("%llu ", best_v);
            ++head[best_r];
        }
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
