// 04 — Sum of N Numbers | v2: chunks stored in PGAS heap + bulk rget verify
// Each rank allocates its chunk in its OWN PGAS heap and fills it locally.
// After the allreduce, rank 0 re-reads every rank's chunk remotely (bulk
// bclx::rget_sync) and re-sums — a verification pass that teaches the
// "distributed array" pattern: alloc + broadcast gptrs + remote read.
// Run: make run PROB=04_sum_of_n_numbers SRC=result/v2_pgas_verify.cpp NP=4 ARGS="1000"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 1000;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <N>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t n = 0;
    if (me == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);

    // my chunk of the distributed array
    uint64_t lo = n * me / P;
    uint64_t hi = n * (me + 1) / P;
    uint64_t len = hi - lo;

    BCL::GlobalPtr<uint64_t> chunk = BCL::alloc<uint64_t>(len > 0 ? len : 1);
    for (uint64_t j = 0; j < len; ++j)
        chunk.local()[j] = value_at(lo + j);   // local store on my own heap

    // share every rank's chunk gptr
    std::vector<BCL::GlobalPtr<uint64_t>> bases(P);
    bases[me] = chunk;
    for (uint64_t r = 0; r < P; ++r)
        bases[r] = BCL::broadcast(bases[r], r);

    uint64_t partial = 0;
    for (uint64_t j = 0; j < len; ++j) partial += chunk.local()[j];
    uint64_t total = bclx::allreduce(partial, BCL::sum<uint64_t>{});

    bclx::barrier_sync();

    // verification: rank 0 re-reads ALL chunks remotely (bulk rget)
    if (me == 0) {
        uint64_t check = 0;
        std::vector<uint64_t> buf;
        for (uint64_t r = 0; r < P; ++r) {
            uint64_t rlo = n * r / P, rhi = n * (r + 1) / P, rlen = rhi - rlo;
            if (rlen == 0) continue;
            buf.resize(rlen);
            bclx::rget_sync(bases[r], buf.data(), rlen);   // one bulk read per rank
            for (uint64_t v : buf) check += v;
        }
        printf("total = %llu, remote re-read = %llu -> %s\n",
               total, check, total == check ? "MATCH" : "MISMATCH!");
    }

    BCL::dealloc<uint64_t>(chunk);
    BCL::finalize();
    return 0;
}
