// 53 — Exactly x Divisors | v2: partition [1..n] + allreduce
// The classic group-B shape: each rank counts matches in its contiguous
// chunk, the counts are summed with allreduce. Per-number work grows with
// sqrt(i), so the last chunk is again the heaviest — the load-balancing
// itch that work stealing scratches (see LoadBalancing/).
// Run: make run PROB=53_exactly_x_divisors SRC=result/v2_allreduce.cpp NP=4 ARGS="10000 4"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t divisor_count(uint64_t n) {
    uint64_t c = 0;
    for (uint64_t i = 1; i * i <= n; ++i)
        if (n % i == 0) c += (i == n / i) ? 1 : 2;
    return c;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n> <x>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t n = 0, x = 0;
    if (me == 0) {
        n = strtoull(argv[1], nullptr, 10);
        x = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    x = BCL::broadcast(x, 0);

    uint64_t my_lo = n * me / P + 1;
    uint64_t my_hi = n * (me + 1) / P;

    uint64_t found = 0;
    for (uint64_t i = my_lo; i <= my_hi; ++i)
        if (divisor_count(i) == x) ++found;

    uint64_t total = bclx::allreduce(found, BCL::sum<uint64_t>{});

    if (me == 0)
        printf("numbers in [1..%llu] with exactly %llu divisors: %llu (parallel over %llu ranks)\n",
               n, x, total, P);

    BCL::finalize();
    return 0;
}
