// 17 — Nth Term of the Fibonacci Series | v2: fast doubling + cross-check
// Two different algorithms on two different ranks, results exchanged through
// rank 0's PGAS heap: rank 0 computes F(n) with fast doubling (O(log n)),
// the LAST rank computes it iteratively (O(n)); rank 0 compares both.
// Fast doubling identities: F(2k) = F(k) * (2*F(k+1) - F(k)),
//                           F(2k+1) = F(k)^2 + F(k+1)^2.
// Run: make run PROB=17_nth_fibonacci SRC=result/v2_fast_doubling.cpp NP=2 ARGS="50"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

// returns F(n) in *f, F(n+1) in *g
static void fib_pair(uint64_t n, uint64_t *f, uint64_t *g) {
    if (n == 0) { *f = 0; *g = 1; return; }
    uint64_t a, b;
    fib_pair(n >> 1, &a, &b);
    uint64_t c = a * (2 * b - a);   // F(2k)
    uint64_t d = a * a + b * b;     // F(2k+1)
    if (n & 1) { *f = d; *g = c + d; }
    else       { *f = c; *g = d; }
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
    if (n > 93) n = 93;

    // rank 0 hosts the two answers
    BCL::GlobalPtr<uint64_t> g = nullptr;
    if (me == 0) {
        g = BCL::alloc<uint64_t>(2);
        g.local()[0] = 0;
        g.local()[1] = 0;
    }
    g = BCL::broadcast(g, 0);

    if (me == 0) {
        uint64_t f, h;
        fib_pair(n, &f, &h);
        bclx::aput_sync(f, g + 0);                 // fast doubling answer
    } else if (me == P - 1) {
        uint64_t a = 0, b = 1;                       // iterative answer
        for (uint64_t i = 0; i < n; ++i) { uint64_t c = a + b; a = b; b = c; }
        bclx::aput_sync(a, g + 1);
    }

    bclx::barrier_sync();

    if (me == 0) {
        uint64_t fd = g.local()[0], it = g.local()[1];
        printf("F(%llu): fast doubling = %llu, iterative = %llu -> %s\n",
               n, fd, it, fd == it ? "MATCH" : "MISMATCH!");
        BCL::dealloc<uint64_t>(g);
    }

    BCL::finalize();
    return 0;
}
