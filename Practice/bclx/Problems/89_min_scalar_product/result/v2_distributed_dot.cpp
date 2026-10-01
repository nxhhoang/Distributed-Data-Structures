// 89 — Minimum Scalar Product | v2: rank 0 ships sorted chunks, local dot + allreduce
// Rank 0 sorts both vectors (ascending / descending as required), then
// distributes each rank's chunk into that rank's PGAS buffer with ONE bulk
// rput per rank per vector — writing TO remote memory, the mirror of 04/v2's
// bulk rget FROM remote memory. Each rank then computes the partial dot of
// its chunk locally; the partials are summed with allreduce.
// Run: make run PROB=89_min_scalar_product SRC=result/v2_distributed_dot.cpp NP=4 ARGS="16"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
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

    // my chunk buffers in MY PGAS heap
    uint64_t lo = n * me / P, hi = n * (me + 1) / P;
    uint64_t len = hi - lo;
    BCL::GlobalPtr<uint64_t> buf_a = BCL::alloc<uint64_t>(len > 0 ? len : 1);
    BCL::GlobalPtr<uint64_t> buf_b = BCL::alloc<uint64_t>(len > 0 ? len : 1);

    std::vector<BCL::GlobalPtr<uint64_t>> bases_a(P), bases_b(P);
    bases_a[me] = buf_a;
    bases_b[me] = buf_b;
    for (uint64_t r = 0; r < P; ++r) {
        bases_a[r] = BCL::broadcast(bases_a[r], r);
        bases_b[r] = BCL::broadcast(bases_b[r], r);
    }

    bclx::barrier_sync();

    // rank 0: sort both, then bulk-ship each rank's chunk
    if (me == 0) {
        std::vector<uint64_t> a(n), b(n);
        for (uint64_t i = 0; i < n; ++i) {
            a[i] = value_at(i);
            b[i] = value_at(i + 7777);
        }
        std::sort(a.begin(), a.end());                            // ascending
        std::sort(b.begin(), b.end(), std::greater<uint64_t>()); // descending

        for (uint64_t r = 0; r < P; ++r) {
            uint64_t rlo = n * r / P, rhi = n * (r + 1) / P, rlen = rhi - rlo;
            if (rlen == 0) continue;
            bclx::rput_sync(a.data() + rlo, bases_a[r], rlen);   // bulk write TO rank r
            bclx::rput_sync(b.data() + rlo, bases_b[r], rlen);
        }
    }

    bclx::barrier_sync();

    // local partial dot over my chunk
    uint64_t partial = 0;
    for (uint64_t j = 0; j < len; ++j)
        partial += buf_a.local()[j] * buf_b.local()[j];

    uint64_t total = bclx::allreduce(partial, BCL::sum<uint64_t>{});

    if (me == 0)
        printf("minimum scalar product = %llu (distributed dot over %llu ranks)\n", total, P);

    BCL::dealloc<uint64_t>(buf_a);
    BCL::dealloc<uint64_t>(buf_b);
    BCL::finalize();
    return 0;
}
