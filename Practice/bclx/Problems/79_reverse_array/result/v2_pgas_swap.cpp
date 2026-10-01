// 79 — Reverse an Array | v2: distributed mirrored swap
// The array lives in the ranks' PGAS heaps as one distributed array. Each
// rank agets the values of its MIRRORED region (element i needs the old
// value of n-1-i), holds them in a local buffer, and only after a barrier
// writes them into its own slots — read everything first, write after, so
// no rank overwrites a value somebody still needs. The final state is
// verified against the deterministic original (a_new[i] == value_at(n-1-i)).
// Run: make run PROB=79_reverse_array SRC=result/v2_pgas_swap.cpp NP=4 ARGS="16"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
}

// owner of global index m under contiguous chunking [n*r/P, n*(r+1)/P)
static uint64_t owner_of(uint64_t m, uint64_t n, uint64_t P) {
    for (uint64_t r = 0; r < P; ++r)
        if (m >= n * r / P && m < n * (r + 1) / P) return r;
    return P - 1;
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
        chunk.local()[j] = value_at(lo + j);   // a[i] = original

    std::vector<BCL::GlobalPtr<uint64_t>> bases(P);
    bases[me] = chunk;
    for (uint64_t r = 0; r < P; ++r)
        bases[r] = BCL::broadcast(bases[r], r);

    // phase 1: READ the mirrored region into a local buffer
    std::vector<uint64_t> buf(len);
    for (uint64_t i = lo; i < hi; ++i) {
        uint64_t m = n - 1 - i;                 // mirror index
        uint64_t r = owner_of(m, n, P);
        buf[i - lo] = bclx::aget_sync(bases[r] + (m - n * r / P));
    }

    bclx::barrier_sync();                       // everyone has read the old values

    // phase 2: WRITE them into my own slots
    for (uint64_t j = 0; j < len; ++j)
        chunk.local()[j] = buf[j];

    bclx::barrier_sync();

    // verification: a_new[i] must equal the original a[n-1-i]
    uint64_t bad = 0;
    for (uint64_t i = lo; i < hi; ++i)
        if (chunk.local()[i - lo] != value_at(n - 1 - i)) ++bad;
    uint64_t total_bad = bclx::allreduce(bad, BCL::sum<uint64_t>{});

    if (me == 0)
        printf("distributed reverse of %llu elements: %s\n",
               n, total_bad == 0 ? "verified" : "MISMATCH!");

    BCL::dealloc<uint64_t>(chunk);
    BCL::finalize();
    return 0;
}
