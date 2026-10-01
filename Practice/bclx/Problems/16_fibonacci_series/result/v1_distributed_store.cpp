// 16 — Fibonacci Series Upto nth Term | v1: sequential compute, DISTRIBUTED storage
// The series itself is sequential (term i depends on i-1 and i-2 — see the
// README's group C), but the STORAGE is a distributed array: term i lives at
// rank (i % P), slot (i / P). Rank 0 computes the terms and writes them
// remotely with aput_sync; afterwards every rank verifies its own share by
// recomputing locally — the write/read round-trip of a distributed array.
// F(93) is the largest term that fits in uint64.
// Run: make run PROB=16_fibonacci_series SRC=result/v1_distributed_store.cpp NP=4 ARGS="50"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n_terms>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t n = 0;
    if (me == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n > 93) n = 93;   // uint64 overflow guard

    // my share of the distributed array: terms with i % P == me
    uint64_t slots = (n + P - 1) / P;
    BCL::GlobalPtr<uint64_t> mine = BCL::alloc<uint64_t>(slots > 0 ? slots : 1);

    std::vector<BCL::GlobalPtr<uint64_t>> bases(P);
    bases[me] = mine;
    for (uint64_t r = 0; r < P; ++r)
        bases[r] = BCL::broadcast(bases[r], r);

    // rank 0 computes the series and stores each term in its home slot
    if (me == 0) {
        uint64_t a = 0, b = 1;
        for (uint64_t i = 0; i < n; ++i) {
            bclx::aput_sync(a, bases[i % P] + i / P);   // remote write for i%P != 0
            uint64_t c = a + b;
            a = b;
            b = c;
        }
    }

    bclx::barrier_sync();

    // every rank recomputes the series locally and checks ITS share
    std::vector<uint64_t> fib(n);
    {
        uint64_t a = 0, b = 1;
        for (uint64_t i = 0; i < n; ++i) { fib[i] = a; uint64_t c = a + b; a = b; b = c; }
    }
    uint64_t bad = 0;
    for (uint64_t i = me; i < n; i += P)
        if (mine.local()[i / P] != fib[i]) ++bad;

    uint64_t total_bad = bclx::allreduce(bad, BCL::sum<uint64_t>{});

    if (me == 0) {
        printf("first terms: ");
        for (uint64_t i = 0; i < n && i < 10; ++i)
            printf("%llu ", bclx::aget_sync(bases[i % P] + i / P));   // remote read
        printf("... (%s)\n", total_bad == 0 ? "distributed store verified" : "MISMATCH!");
    }

    BCL::dealloc<uint64_t>(mine);
    BCL::finalize();
    return 0;
}
