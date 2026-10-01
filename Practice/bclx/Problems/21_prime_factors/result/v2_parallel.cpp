// 21 — Finding Prime Factors of a Number | v2: parallel divisor discovery
// The search for DISTINCT prime divisors is parallel: each rank stripes the
// candidates in [2..sqrt(n)] (d = 2+me, step P) and reports candidates that
// both divide n and are themselves prime, using the fao+aput atomic-append
// pattern (see 10/v2). Assembling the multiplicities is then trivial and
// sequential on rank 0.
// Run: make run PROB=21_prime_factors SRC=result/v2_parallel.cpp NP=4 ARGS="360360"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <vector>
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

    uint64_t root = (uint64_t)sqrtl((long double)n);
    while ((root + 1) * (root + 1) <= n) ++root;

    // rank 0 hosts the collected distinct prime factors + tail
    BCL::GlobalPtr<uint64_t> facs = nullptr, tail = nullptr;
    if (me == 0) {
        facs = BCL::alloc<uint64_t>(64);   // u64 has at most 15 distinct primes
        tail = BCL::alloc<uint64_t>(1);
        *tail.local() = 0;
    }
    facs = BCL::broadcast(facs, 0);
    tail = BCL::broadcast(tail, 0);

    for (uint64_t d = 2 + me; d <= root; d += P) {
        if (n % d == 0 && is_prime(d)) {
            uint64_t idx = bclx::fao_sync(tail, uint64_t(1), BCL::plus<uint64_t>{});
            bclx::aput_sync(d, facs + idx);
        }
    }

    bclx::barrier_sync();

    if (me == 0) {
        uint64_t k = tail.local()[0];
        std::vector<uint64_t> fs(facs.local(), facs.local() + k);
        std::sort(fs.begin(), fs.end());   // parallel completion order is random

        printf("%llu = ", n);
        uint64_t x = n;
        for (size_t i = 0; i < fs.size(); ++i) {
            uint64_t p = fs[i], mult = 0;
            while (x % p == 0) { x /= p; ++mult; }
            printf(i ? " * " : "");
            if (mult > 1) printf("%llu^%llu", p, mult);
            else          printf("%llu", p);
        }
        if (x > 1) printf(fs.empty() ? "%llu" : " * %llu", x);
        printf("\n");
        BCL::dealloc<uint64_t>(facs);
        BCL::dealloc<uint64_t>(tail);
    }

    BCL::finalize();
    return 0;
}
