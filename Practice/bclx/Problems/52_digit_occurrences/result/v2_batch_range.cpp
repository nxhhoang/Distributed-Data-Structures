// 52 — Digit Occurrences | v2: count over a whole range [L..R] with fao
// Each rank scans its contiguous chunk of numbers and counts occurrences of
// digit x inside each number; the totals are accumulated with fao on rank
// 0's counter (the batch pattern from 02/v2).
// Run: make run PROB=52_digit_occurrences SRC=result/v2_batch_range.cpp NP=4 ARGS="1 1000 7"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t count_digit(uint64_t v, uint64_t x) {
    uint64_t c = 0;
    for (; v > 0; v /= 10)
        if (v % 10 == x) ++c;
    return c;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "usage: %s <L> <R> <digit x>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t lo = 0, hi = 0, x = 0;
    if (me == 0) {
        lo = strtoull(argv[1], nullptr, 10);
        hi = strtoull(argv[2], nullptr, 10);
        x = strtoull(argv[3], nullptr, 10);
    }
    lo = BCL::broadcast(lo, 0);
    hi = BCL::broadcast(hi, 0);
    x = BCL::broadcast(x, 0);

    BCL::GlobalPtr<uint64_t> total = nullptr;
    if (me == 0) {
        total = BCL::alloc<uint64_t>(1);
        *total.local() = 0;
    }
    total = BCL::broadcast(total, 0);

    uint64_t len = hi - lo + 1;
    uint64_t my_lo = lo + me * len / P;
    uint64_t my_hi = lo + (me + 1) * len / P - 1;

    for (uint64_t v = my_lo; v <= my_hi; ++v) {
        uint64_t c = count_digit(v, x);
        if (c > 0) bclx::fao_sync(total, c, BCL::plus<uint64_t>{});
    }

    bclx::barrier_sync();

    if (me == 0) {
        printf("digit %llu occurs %llu time%s across [%llu..%llu]\n",
               x, total.local()[0], total.local()[0] == 1 ? "" : "s", lo, hi);
        BCL::dealloc<uint64_t>(total);
    }

    BCL::finalize();
    return 0;
}
