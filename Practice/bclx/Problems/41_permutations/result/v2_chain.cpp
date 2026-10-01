// 41 — Permutations: n People Occupy r Seats | v2: striped factors + chain
// The r factors {n, n-1, ..., n-r+1} are striped over the ranks (factor index
// i with i % P == me); each rank multiplies its own subset, then the partial
// products are chained rank by rank with aget/aput — the same "mailbox"
// pattern as 18/v2, but for multiplication (no allreduce-with-product op).
// Run: make run PROB=41_permutations SRC=result/v2_chain.cpp NP=4 ARGS="10 6"

#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <vector>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n> <r>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t n = 0, r = 0;
    if (me == 0) {
        n = strtoull(argv[1], nullptr, 10);
        r = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    r = BCL::broadcast(r, 0);

    if (r > n) {
        if (me == 0) printf("P(%llu, %llu) = 0\n", n, r);
        BCL::finalize();
        return 0;
    }

    // my striped share of the falling factors n, n-1, ..., n-r+1
    uint64_t partial = 1;
    for (uint64_t i = me; i < r; i += P)
        partial *= (n - i);

    // chain the partials: rank r waits for rank r-1's mailbox
    std::vector<BCL::GlobalPtr<uint64_t>> box(P);
    box[me] = BCL::alloc<uint64_t>(1);
    *box[me].local() = 0;   // 0 = not ready (products are always >= 1)
    for (uint64_t r2 = 0; r2 < P; ++r2)
        box[r2] = BCL::broadcast(box[r2], r2);

    if (me == 0) {
        bclx::aput_sync(partial, box[0]);
    } else {
        uint64_t prev;
        while ((prev = bclx::aget_sync(box[me - 1])) == 0)
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        bclx::aput_sync(prev * partial, box[me]);
    }

    bclx::barrier_sync();

    if (me == 0) {
        uint64_t p = bclx::aget_sync(box[P - 1]);
        printf("P(%llu, %llu) = %llu (chained over %llu ranks)\n", n, r, p, P);
    }

    BCL::dealloc<uint64_t>(box[me]);
    BCL::finalize();
    return 0;
}
