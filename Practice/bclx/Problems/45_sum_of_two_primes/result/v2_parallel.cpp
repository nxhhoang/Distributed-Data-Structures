// 45 — Can a Number be Expressed as a Sum of Two Primes | v2: striped search
// Candidate primes p are striped across the ranks (p = 2+me, step P); the
// FIRST rank to find a working pair claims the reporting slot with a single
// fao(+1) — the old value tells it whether it is first (0) or not — and
// writes the pair with aput. Later finders see a non-zero old value and stay
// silent. This is the "first reporter wins" pattern.
// Run: make run PROB=45_sum_of_two_primes SRC=result/v2_parallel.cpp NP=4 ARGS="74"

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
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t n = 0;
    if (me == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);

    BCL::GlobalPtr<uint64_t> found = nullptr, pair = nullptr;
    if (me == 0) {
        found = BCL::alloc<uint64_t>(1);
        pair = BCL::alloc<uint64_t>(2);
        *found.local() = 0;
    }
    found = BCL::broadcast(found, 0);
    pair = BCL::broadcast(pair, 0);

    // each rank scans its stripe of candidates
    for (uint64_t p = 2 + me; p <= n / 2; p += P) {
        if (is_prime(p) && is_prime(n - p)) {
            if (bclx::fao_sync(found, uint64_t(1), BCL::plus<uint64_t>{}) == 0) {
                bclx::aput_sync(p, pair + 0);       // first reporter wins
                bclx::aput_sync(n - p, pair + 1);
            }
            break;   // this rank is done
        }
    }

    bclx::barrier_sync();

    if (me == 0) {
        if (found.local()[0] > 0)
            printf("%llu = %llu + %llu (found by a striped search)\n",
                   n, pair.local()[0], pair.local()[1]);
        else
            printf("%llu cannot be expressed as a sum of two primes\n", n);
        BCL::dealloc<uint64_t>(found);
        BCL::dealloc<uint64_t>(pair);
    }

    BCL::finalize();
    return 0;
}
