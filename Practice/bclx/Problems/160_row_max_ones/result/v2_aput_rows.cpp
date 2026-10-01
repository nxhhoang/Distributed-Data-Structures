// 160 — Row With the Maximum Number of 1's | v2: striped rows + aput pairs
// Rank me evaluates its striped rows (r % P == me), keeps its local best
// (count, row) and ships it with aput into its slot of rank 0's parts array.
// Rank 0 combines — the composite aput-reduce of 76/v2 on matrix rows.
// Run: make run PROB=160_row_max_ones SRC=result/v2_aput_rows.cpp NP=4 ARGS="20 8"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t first_one(uint64_t r, uint64_t C) {
    return (r * 3 + 1) % C;
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

    // my striped rows' best (count, row)
    uint64_t best_count = 0, best_row = 0;
    for (uint64_t r = me; r < R; r += P) {
        uint64_t lo = 0, hi = C;   // bsearch for the first 1
        while (lo < hi) {
            uint64_t mid = lo + (hi - lo) / 2;
            if (mid >= first_one(r, C)) hi = mid;
            else lo = mid + 1;
        }
        uint64_t count = C - lo;
        if (count > best_count) { best_count = count; best_row = r; }
    }

    // rank 0 hosts P slots of (count, row)
    BCL::GlobalPtr<uint64_t> parts = nullptr;
    if (me == 0) {
        parts = BCL::alloc<uint64_t>(2 * P);
        for (uint64_t r = 0; r < 2 * P; ++r) parts.local()[r] = 0;
    }
    parts = BCL::broadcast(parts, 0);

    bclx::aput_sync(best_count, parts + 2 * me);
    bclx::aput_sync(best_row, parts + 2 * me + 1);
    bclx::barrier_sync();

    if (me == 0) {
        uint64_t gc = 0, gr = 0;
        for (uint64_t r = 0; r < P; ++r) {
            uint64_t c = parts.local()[2 * r], row = parts.local()[2 * r + 1];
            if (c > gc || (c == gc && row < gr)) { gc = c; gr = row; }
        }
        printf("row %llu has the most 1's: %llu of %llu (striped rows over %llu ranks)\n",
               gr, gc, C, P);
        BCL::dealloc<uint64_t>(parts);
    }

    BCL::finalize();
    return 0;
}
