// 91 — Count Even and Odd Elements | v2: chunk counts + fao counters
// The 02/v2 pattern applied to array data: each rank scans its chunk of the
// distributed array and faos its local even/odd counts straight into rank
// 0's counters (batched: ONE fao per rank per parity, not per element).
// Run: make run PROB=91_count_even_odd SRC=result/v2_fao_counters.cpp NP=4 ARGS="30"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t n = 0;
    if (me == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n == 0) n = 1;

    BCL::GlobalPtr<uint64_t> cnt = nullptr;
    if (me == 0) {
        cnt = BCL::alloc<uint64_t>(2);
        cnt.local()[0] = 0;   // even
        cnt.local()[1] = 0;   // odd
    }
    cnt = BCL::broadcast(cnt, 0);

    uint64_t lo = n * me / P, hi = n * (me + 1) / P;
    uint64_t my_even = 0, my_odd = 0;
    for (uint64_t i = lo; i < hi; ++i) {
        if (value_at(i) % 2 == 0) ++my_even;
        else ++my_odd;
    }

    // batched: one fao per rank per counter
    if (my_even > 0) bclx::fao_sync(cnt + 0, my_even, BCL::plus<uint64_t>{});
    if (my_odd > 0)  bclx::fao_sync(cnt + 1, my_odd,  BCL::plus<uint64_t>{});

    bclx::barrier_sync();

    if (me == 0) {
        printf("even = %llu, odd = %llu (of %llu, fao-reduced)\n",
               cnt.local()[0], cnt.local()[1], n);
        BCL::dealloc<uint64_t>(cnt);
    }

    BCL::finalize();
    return 0;
}
