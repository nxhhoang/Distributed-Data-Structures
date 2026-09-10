// 02 — Even or Odd | v2: batch over a range + fao counters on rank 0
// Each rank scans its contiguous chunk of [LO..HI] and atomically increments
// the even / odd counters living in rank 0's PGAS heap.
// Primitives: BCL::alloc, gptr broadcast, bclx::fao_sync (returns old value).
// Run: make run PROB=02_even_or_odd SRC=result/v2_batch_count.cpp NP=4 ARGS="1 100"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <lo> <hi>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    // shared counters live in rank 0's PGAS heap
    BCL::GlobalPtr<uint64_t> cnt = nullptr;
    if (me == 0) {
        cnt = BCL::alloc<uint64_t>(2);
        cnt.local()[0] = 0;   // even
        cnt.local()[1] = 0;   // odd
    }
    cnt = BCL::broadcast(cnt, 0);   // gptrs are 8 bytes -> broadcastable

    uint64_t lo = strtoull(argv[1], nullptr, 10);
    uint64_t hi = strtoull(argv[2], nullptr, 10);

    // partition [lo..hi] into P contiguous chunks
    uint64_t len = hi - lo + 1;
    uint64_t my_lo = lo + me * len / P;
    uint64_t my_hi = lo + (me + 1) * len / P - 1;

    for (uint64_t x = my_lo; x <= my_hi; ++x) {
        if (x % 2 == 0) bclx::fao_sync(cnt + 0, uint64_t(1), BCL::plus<uint64_t>{});
        else            bclx::fao_sync(cnt + 1, uint64_t(1), BCL::plus<uint64_t>{});
    }

    bclx::barrier_sync();
    if (me == 0) {
        printf("[%llu..%llu]: %llu even, %llu odd\n",
               lo, hi, cnt.local()[0], cnt.local()[1]);
        BCL::dealloc<uint64_t>(cnt);
    }

    BCL::finalize();
    return 0;
}
