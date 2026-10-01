// 57 — Largest Element in an Array | v2: PGAS chunks + recursive max + aput
// The array is a distributed array: each rank fills its chunk in its own PGAS
// heap (like 04/v2), finds its chunk maximum with the recursion, and reports
// it with aput into rank 0's partials array; rank 0 takes the final max
// locally — a "manual max-reduce" (there is no allreduce-max for integers in
// the confirmed op set, so we build it ourselves).
// Run: make run PROB=57_recursive_max_array SRC=result/v2_allreduce.cpp NP=4 ARGS="16"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 1000;
}

static uint64_t max_rec(const uint64_t *a, size_t i) {
    if (i == 0) return a[0];
    uint64_t m = max_rec(a, i - 1);
    return a[i] > m ? a[i] : m;
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

    // my chunk of the distributed array
    uint64_t lo = n * me / P, hi = n * (me + 1) / P;
    uint64_t len = hi - lo;
    BCL::GlobalPtr<uint64_t> chunk = BCL::alloc<uint64_t>(len > 0 ? len : 1);
    for (uint64_t j = 0; j < len; ++j)
        chunk.local()[j] = value_at(lo + j);

    uint64_t mymax = len > 0 ? max_rec(chunk.local(), len - 1) : 0;

    // rank 0 hosts the partial maxima
    BCL::GlobalPtr<uint64_t> parts = nullptr;
    if (me == 0) {
        parts = BCL::alloc<uint64_t>(P);
        for (uint64_t r = 0; r < P; ++r) parts.local()[r] = 0;
    }
    parts = BCL::broadcast(parts, 0);

    bclx::aput_sync(mymax, parts + me);
    bclx::barrier_sync();

    if (me == 0) {
        uint64_t m = 0;
        for (uint64_t r = 0; r < P; ++r)
            if (parts.local()[r] > m) m = parts.local()[r];
        printf("largest of %llu elements = %llu (recursive max over %llu distributed chunks)\n",
               n, m, P);
        BCL::dealloc<uint64_t>(parts);
    }

    BCL::dealloc<uint64_t>(chunk);
    BCL::finalize();
    return 0;
}
