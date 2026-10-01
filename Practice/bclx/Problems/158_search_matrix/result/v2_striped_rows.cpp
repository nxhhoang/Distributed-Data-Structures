// 158 — Search an Element in a Matrix | v2: striped rows + bsearch + fao
// The rows of a row-column sorted matrix are sorted, so each rank can binary
// search its striped rows (r % P == me) independently; the FIRST reporter
// takes the reporting slot with fao(+1) (old value 0) and aputs the position.
// (In this strictly-sorted matrix the target occurs at most once; with
// duplicates, every rank would report and the last write would win.)
// Run: make run PROB=158_search_matrix SRC=result/v2_striped_rows.cpp NP=4 ARGS="40 50 231"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t mat_at(uint64_t r, uint64_t c) {
    return 10 * r + 2 * c + 1;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "usage: %s <rows> <cols> <target>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t R = 0, C = 0, target = 0;
    if (me == 0) {
        R = strtoull(argv[1], nullptr, 10);
        C = strtoull(argv[2], nullptr, 10);
        target = strtoull(argv[3], nullptr, 10);
    }
    R = BCL::broadcast(R, 0);
    C = BCL::broadcast(C, 0);
    target = BCL::broadcast(target, 0);
    if (R == 0) R = 1;
    if (C == 0) C = 1;

    BCL::GlobalPtr<uint64_t> found = nullptr, pos = nullptr;
    if (me == 0) {
        found = BCL::alloc<uint64_t>(1);
        pos = BCL::alloc<uint64_t>(2);
        *found.local() = 0;
    }
    found = BCL::broadcast(found, 0);
    pos = BCL::broadcast(pos, 0);

    // binary search my striped rows
    for (uint64_t r = me; r < R; r += P) {
        uint64_t lo = 0, hi = C;   // first index with mat > target
        while (lo < hi) {
            uint64_t mid = lo + (hi - lo) / 2;
            if (mat_at(r, mid) <= target) lo = mid + 1;
            else hi = mid;
        }
        if (lo > 0 && mat_at(r, lo - 1) == target) {
            if (bclx::fao_sync(found, uint64_t(1), BCL::plus<uint64_t>{}) == 0) {
                bclx::aput_sync(r, pos + 0);          // first reporter wins
                bclx::aput_sync(lo - 1, pos + 1);
            }
            break;
        }
    }

    bclx::barrier_sync();

    if (me == 0) {
        if (found.local()[0] > 0)
            printf("%llu found at row %llu, col %llu (striped rows over %llu ranks)\n",
                   target, pos.local()[0], pos.local()[1], P);
        else
            printf("%llu not in the matrix\n", target);
        BCL::dealloc<uint64_t>(found);
        BCL::dealloc<uint64_t>(pos);
    }

    BCL::finalize();
    return 0;
}
