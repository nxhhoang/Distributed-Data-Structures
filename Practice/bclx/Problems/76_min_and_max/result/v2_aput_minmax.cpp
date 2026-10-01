// 76 — Smallest and Largest Element | v2: chunk min/max + aput pairs
// The array is a distributed array (each rank fills its chunk in its own PGAS
// heap); every rank finds BOTH extrema of its chunk in one local pass and
// reports the pair with aput into rank 0's per-rank slots. Rank 0 combines
// the P pairs — a composite (two-word) reduction built by hand.
// Run: make run PROB=76_min_and_max SRC=result/v2_aput_minmax.cpp NP=4 ARGS="16"

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

    uint64_t lo = n * me / P, hi = n * (me + 1) / P;
    uint64_t len = hi - lo;

    uint64_t mn = ~0ull, mx = 0;
    for (uint64_t i = lo; i < hi; ++i) {
        uint64_t v = value_at(i);
        if (v < mn) mn = v;
        if (v > mx) mx = v;
    }
    if (len == 0) { mn = 0; mx = 0; }

    // rank 0 hosts P slots of (min, max)
    BCL::GlobalPtr<uint64_t> parts = nullptr;
    if (me == 0) {
        parts = BCL::alloc<uint64_t>(2 * P);
        for (uint64_t r = 0; r < 2 * P; ++r) parts.local()[r] = 0;
    }
    parts = BCL::broadcast(parts, 0);

    bclx::aput_sync(mn, parts + 2 * me);       // my min slot
    bclx::aput_sync(mx, parts + 2 * me + 1);   // my max slot
    bclx::barrier_sync();

    if (me == 0) {
        uint64_t gmn = parts.local()[0], gmx = parts.local()[1];
        for (uint64_t r = 1; r < P; ++r) {
            if (parts.local()[2 * r] < gmn) gmn = parts.local()[2 * r];
            if (parts.local()[2 * r + 1] > gmx) gmx = parts.local()[2 * r + 1];
        }
        printf("smallest = %llu, largest = %llu (composite aput-reduce over %llu ranks)\n",
               gmn, gmx, P);
        BCL::dealloc<uint64_t>(parts);
    }

    BCL::finalize();
    return 0;
}
