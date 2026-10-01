// 48 — Find the Prime Numbers Between 1 and 100 | v1: STRIPED partition
// Candidate x belongs to rank (x mod P) — compare with problem 10/v1 which
// used CONTIGUOUS chunks. Striping spreads the more expensive (larger)
// numbers evenly over the ranks; contiguous chunks put them all at the end.
// Run: make run PROB=48_primes_1_to_100 SRC=result/v1_striped.cpp NP=4

#include <cstdio>
#include <bclx/bclx.hpp>

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    for (uint64_t d = 2; d * d <= n; ++d)
        if (n % d == 0) return false;
    return true;
}

int main() {
    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    // striped: x = 2+me, 2+me+P, ... up to 100
    uint64_t found = 0;
    for (uint64_t x = 2 + me; x <= 100; x += P) {
        if (is_prime(x)) {
            printf("[rank %llu] %llu\n", me, x);
            ++found;
        }
    }

    uint64_t total = bclx::allreduce(found, BCL::sum<uint64_t>{});
    if (me == 0)
        printf("total primes in [1..100]: %llu (striped over %llu ranks)\n", total, P);

    BCL::finalize();
    return 0;
}
