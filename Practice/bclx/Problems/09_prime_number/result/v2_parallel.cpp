// 09 — Prime Number | v2: parallel trial division
// Candidate divisors in [2..sqrt(n)] are striped across the ranks
// (d = 2 + me, step P); a hit on any rank means composite. The local hit
// counts are combined with allreduce — zero total hits means prime.
// Workload per rank drops to sqrt(n)/P.
// Run: make run PROB=09_prime_number SRC=result/v2_parallel.cpp NP=4 ARGS="1000000007"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <bclx/bclx.hpp>

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

    if (n < 2) {
        if (me == 0) printf("%llu is NOT prime\n", n);
        BCL::finalize();
        return 0;
    }

    uint64_t root = (uint64_t)sqrtl((long double)n);
    while ((root + 1) * (root + 1) <= n) ++root;   // fix rounding up

    // striping: I test d = 2+me, 2+me+P, 2+me+2P, ... up to root
    uint64_t hits = 0;
    for (uint64_t d = 2 + me; d <= root; d += P)
        if (n % d == 0) hits++;

    uint64_t total_hits = bclx::allreduce(hits, BCL::sum<uint64_t>{});
    if (me == 0)
        printf("%llu is %sprime (checked ~%llu divisors, %llu ranks)\n",
               n, total_hits == 0 ? "" : "NOT ", root - 1, P);

    BCL::finalize();
    return 0;
}
