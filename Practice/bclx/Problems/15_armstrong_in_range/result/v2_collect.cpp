// 15 — Armstrong Numbers in a Given Range | v2: fao + aput collection
// Same atomic-append pattern as 10/v2 (see that file for the explanation):
// each rank checks its chunk; on a hit it takes a slot index with fao(+1)
// and writes the number with aput_sync. For a print-per-rank variant see
// problem 10/result/v1_print.cpp.
// Run: make run PROB=15_armstrong_in_range SRC=result/v2_collect.cpp NP=4 ARGS="1 10000"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static bool is_armstrong(uint64_t n) {
    uint64_t digits = 1;
    for (uint64_t x = n; x >= 10; x /= 10) ++digits;

    uint64_t sum = 0;
    for (uint64_t x = n; x > 0; x /= 10) {
        uint64_t p = 1;
        for (uint64_t k = 0; k < digits; ++k) p *= x % 10;
        sum += p;
    }
    return sum == n;
}

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

    BCL::GlobalPtr<uint64_t> results = nullptr, tail = nullptr;
    if (me == 0) {
        results = BCL::alloc<uint64_t>(hi - lo + 1);
        tail = BCL::alloc<uint64_t>(1);
        *tail.local() = 0;
    }
    results = BCL::broadcast(results, 0);
    tail = BCL::broadcast(tail, 0);

    uint64_t len = hi - lo + 1;
    uint64_t my_lo = lo + me * len / P;
    uint64_t my_hi = lo + (me + 1) * len / P - 1;

    for (uint64_t x = my_lo; x <= my_hi; ++x) {
        if (is_armstrong(x)) {
            uint64_t idx = bclx::fao_sync(tail, uint64_t(1), BCL::plus<uint64_t>{});
            bclx::aput_sync(x, results + idx);
        }
    }

    bclx::barrier_sync();

    if (me == 0) {
        uint64_t n = tail.local()[0];
        printf("Armstrong numbers in [%llu..%llu]: %llu found\n", lo, hi, n);
        for (uint64_t i = 0; i < n; ++i)
            printf("%llu ", results.local()[i]);
        printf("\n");
        BCL::dealloc<uint64_t>(results);
        BCL::dealloc<uint64_t>(tail);
    }

    BCL::finalize();
    return 0;
}
