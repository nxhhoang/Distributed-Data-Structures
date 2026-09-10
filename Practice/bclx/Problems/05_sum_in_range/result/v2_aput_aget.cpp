// 05 — Sum of Numbers in a Given Range | v2: "manual reduce" with aput/aget
// Instead of allreduce, every rank writes its partial sum into its slot of a
// P-word array in rank 0's PGAS heap (bclx::aput_sync); after a barrier,
// rank 0 reads them back locally and adds them up. This is what a reduction
// looks like when you build it yourself — a good way to feel what a
// collective hides.
// Run: make run PROB=05_sum_in_range SRC=result/v2_aput_aget.cpp NP=4 ARGS="10 1000000"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <L> <R>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t lo = 0, hi = 0;
    if (me == 0) {
        lo = strtoull(argv[1], nullptr, 10);
        hi = strtoull(argv[2], nullptr, 10);
    }
    lo = BCL::broadcast(lo, 0);
    hi = BCL::broadcast(hi, 0);

    // rank 0 hosts the partials array
    BCL::GlobalPtr<uint64_t> parts = nullptr;
    if (me == 0) {
        parts = BCL::alloc<uint64_t>(P);
        for (uint64_t r = 0; r < P; ++r) parts.local()[r] = 0;
    }
    parts = BCL::broadcast(parts, 0);

    uint64_t len = hi - lo + 1;
    uint64_t my_lo = lo + me * len / P;
    uint64_t my_hi = lo + (me + 1) * len / P - 1;

    uint64_t partial = 0;
    for (uint64_t x = my_lo; x <= my_hi; ++x) partial += x;

    bclx::aput_sync(partial, parts + me);   // my slot, remote write for r != 0

    bclx::barrier_sync();

    if (me == 0) {
        uint64_t total = 0;
        for (uint64_t r = 0; r < P; ++r) total += parts.local()[r];
        printf("sum(%llu..%llu) = %llu\n", lo, hi, total);
        BCL::dealloc<uint64_t>(parts);
    }

    BCL::finalize();
    return 0;
}
