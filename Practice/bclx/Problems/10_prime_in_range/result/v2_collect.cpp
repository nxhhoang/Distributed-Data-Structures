// 10 — Prime Number Within a Given Range | v2: collect results via fao + aput
// The classic "atomic append" pattern: rank 0 hosts a result array and a tail
// counter; a rank that finds a prime takes the next slot index with ONE
// fao(+1) (which returns the OLD value) and writes the prime there with
// aput_sync. No lock, no ordering between ranks.
// Run: make run PROB=10_prime_in_range SRC=result/v2_collect.cpp NP=4 ARGS="2 200"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    for (uint64_t d = 2; d * d <= n; ++d)
        if (n % d == 0) return false;
    return true;
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

    // rank 0 hosts the (upper-bounded) result array + tail index
    BCL::GlobalPtr<uint64_t> results = nullptr, tail = nullptr;
    if (me == 0) {
        results = BCL::alloc<uint64_t>(hi - lo + 1);   // worst case: all prime
        tail = BCL::alloc<uint64_t>(1);
        *tail.local() = 0;
    }
    results = BCL::broadcast(results, 0);
    tail = BCL::broadcast(tail, 0);

    uint64_t len = hi - lo + 1;
    uint64_t my_lo = lo + me * len / P;
    uint64_t my_hi = lo + (me + 1) * len / P - 1;

    for (uint64_t x = my_lo; x <= my_hi; ++x) {
        if (is_prime(x)) {
            uint64_t idx = bclx::fao_sync(tail, uint64_t(1), BCL::plus<uint64_t>{});
            bclx::aput_sync(x, results + idx);
        }
    }

    bclx::barrier_sync();

    if (me == 0) {
        uint64_t n = tail.local()[0];
        printf("primes in [%llu..%llu]: %llu found\n", lo, hi, n);
        printf("first few (in completion order, not sorted): ");
        for (uint64_t i = 0; i < n && i < 10; ++i)
            printf("%llu ", results.local()[i]);
        printf("\n");
        BCL::dealloc<uint64_t>(results);
        BCL::dealloc<uint64_t>(tail);
    }

    BCL::finalize();
    return 0;
}
