// 28 — Friendly Pair | v2: split the work across exactly two ranks
// Rank 0 computes sigma(a), rank 1 computes sigma(b); both results are
// written into rank 0's PGAS heap with aput_sync and compared there after
// a barrier. A miniature "each rank owns one item" pattern.
// Requires NP >= 2. Run: make run PROB=28_friendly_pair SRC=result/v2_two_ranks.cpp NP=2 ARGS="30 140"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t divisor_sum(uint64_t n) {
    if (n == 0) return 0;
    uint64_t s = 0;
    for (uint64_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            s += i;
            if (i != n / i) s += n / i;
        }
    }
    return s;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <a> <b>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    if (P < 2) {
        if (me == 0) fprintf(stderr, "this variant needs NP >= 2\n");
        BCL::finalize();
        return 1;
    }

    uint64_t a = 0, b = 0;
    if (me == 0) {
        a = strtoull(argv[1], nullptr, 10);
        b = strtoull(argv[2], nullptr, 10);
    }
    a = BCL::broadcast(a, 0);
    b = BCL::broadcast(b, 0);

    BCL::GlobalPtr<uint64_t> g = nullptr;
    if (me == 0) {
        g = BCL::alloc<uint64_t>(2);
        g.local()[0] = 0;
        g.local()[1] = 0;
    }
    g = BCL::broadcast(g, 0);

    if (me == 0)      bclx::aput_sync(divisor_sum(a), g + 0);
    else if (me == 1) bclx::aput_sync(divisor_sum(b), g + 1);

    bclx::barrier_sync();

    if (me == 0) {
        uint64_t sa = g.local()[0], sb = g.local()[1];
        unsigned __int128 lhs = (unsigned __int128)sa * b;
        unsigned __int128 rhs = (unsigned __int128)sb * a;
        bool friendly = (a > 0 && b > 0 && lhs == rhs);
        printf("(%llu, %llu) is %sa friendly pair (sigma computed on 2 ranks: %llu, %llu)\n",
               a, b, friendly ? "" : "NOT", sa, sb);
        BCL::dealloc<uint64_t>(g);
    }

    BCL::finalize();
    return 0;
}
