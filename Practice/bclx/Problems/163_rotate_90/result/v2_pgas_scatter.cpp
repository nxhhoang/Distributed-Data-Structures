// 163 — Rotate a Matrix by 90 Degrees | v2: PGAS scatter-write of a permutation
// The matrix lives as one distributed array (n*n u64 slots, contiguous
// chunks per rank). Rotation is the BIJECTIVE index map
// (r, c) -> (c, n-1-r), so every destination cell is written exactly once —
// no conflicts. Protocol (the scatter twin of 79/v2's mirrored swap):
//   phase 1: every rank copies ITS chunk out to a local buffer
//   barrier  (nobody may read a cell someone is about to overwrite)
//   phase 2: each rank aputs its buffered elements to their DESTINATION
//            owners (scattered over many ranks)
// Verification: cell (r', c') must now hold the original (n-1-c', r').
// Run: make run PROB=163_rotate_90 SRC=result/v2_pgas_scatter.cpp NP=4 ARGS="5"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t orig_at(uint64_t r, uint64_t c) {
    return r * 100 + c;   // (row, col) readable directly in the value
}

static uint64_t owner_of(uint64_t m, uint64_t n, uint64_t P) {
    for (uint64_t r = 0; r < P; ++r)
        if (m >= n * r / P && m < n * (r + 1) / P) return r;
    return P - 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n (square)>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t n = 0;
    if (me == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n == 0) n = 1;

    uint64_t N = n * n;   // total flat size
    uint64_t lo = N * me / P, hi = N * (me + 1) / P;
    uint64_t len = hi - lo;
    BCL::GlobalPtr<uint64_t> chunk = BCL::alloc<uint64_t>(len > 0 ? len : 1);
    for (uint64_t i = lo; i < hi; ++i)
        chunk.local()[i - lo] = orig_at(i / n, i % n);

    std::vector<BCL::GlobalPtr<uint64_t>> bases(P);
    bases[me] = chunk;
    for (uint64_t r = 0; r < P; ++r)
        bases[r] = BCL::broadcast(bases[r], r);

    // phase 1: buffer my ORIGINAL values
    std::vector<uint64_t> buf(chunk.local(), chunk.local() + (len ? len : 1));

    bclx::barrier_sync();

    // phase 2: scatter-write each element to its destination owner
    for (uint64_t i = lo; i < hi; ++i) {
        uint64_t r = i / n, c = i % n;
        uint64_t j = c * n + (n - 1 - r);   // dest flat index (bijective)
        uint64_t dst_rank = owner_of(j, N, P);
        bclx::aput_sync(buf[i - lo], bases[dst_rank] + (j - N * dst_rank / P));
    }

    bclx::barrier_sync();

    // verification: cell (r', c') must now hold original (n-1-c', r')
    uint64_t bad = 0;
    for (uint64_t i = lo; i < hi; ++i) {
        uint64_t r = i / n, c = i % n;
        if (chunk.local()[i - lo] != orig_at(n - 1 - c, r)) ++bad;
    }
    uint64_t total_bad = bclx::allreduce(bad, BCL::sum<uint64_t>{});

    if (me == 0) {
        printf("rotated %llux%llu matrix via scatter-write: %s\n",
               n, n, total_bad == 0 ? "verified" : "MISMATCH!");
    }

    BCL::dealloc<uint64_t>(chunk);
    BCL::finalize();
    return 0;
}
