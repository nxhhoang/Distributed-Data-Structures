// 165 — Common Elements in All Rows of a Given Matrix | v1: running intersection
// Every row is sorted. Intersect row 0 with each following row via a merge
// walk; the running intersection only shrinks. The data guarantees some
// common elements (7, 21, 35 appear in every row).
// Run: make run PROB=165_common_all_rows SRC=result/v1_basic.cpp NP=4 ARGS="5 8"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

// row r: sorted union of {7, 21, 35} and row-specific values 1000*r + c
static std::vector<uint64_t> row_values(uint64_t r, uint64_t C) {
    std::vector<uint64_t> v = {7, 21, 35};
    for (uint64_t c = 0; c + 3 < C; ++c)
        v.push_back(1000 * r + c);
    std::sort(v.begin(), v.end());
    return v;
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
    if (C < 4) C = 4;

    if (BCL::rank() == 0) {
        std::vector<std::vector<uint64_t>> rows(R);
        for (uint64_t r = 0; r < R; ++r) rows[r] = row_values(r, C);

        // running intersection, starting from row 0
        std::vector<uint64_t> inter = rows[0];
        for (uint64_t r = 1; r < R; ++r) {
            std::vector<uint64_t> next;
            uint64_t i = 0, j = 0;
            while (i < inter.size() && j < rows[r].size()) {
                if (inter[i] < rows[r][j]) ++i;
                else if (rows[r][j] < inter[i]) ++j;
                else { next.push_back(inter[i]); ++i; ++j; }
            }
            inter = next;
        }

        printf("common elements in all %llu rows: ", R);
        for (uint64_t v : inter) printf("%llu ", v);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
