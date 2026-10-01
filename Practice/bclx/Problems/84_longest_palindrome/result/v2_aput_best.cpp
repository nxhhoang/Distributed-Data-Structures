// 84 — Longest Palindrome in an Array | v2: chunk best + aput (digits, value)
// Every rank scans its chunk of the distributed array, keeps its local best
// (most digits, then larger value), and ships the (digits, value) pair into
// its slot of rank 0's parts array with aput. Rank 0 combines — the same
// composite aput-reduce as 76/v2.
// Run: make run PROB=84_longest_palindrome SRC=result/v2_aput_best.cpp NP=4 ARGS="60"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 150;
}

static uint64_t digits_of(uint64_t v) {
    uint64_t d = 1;
    for (; v >= 10; v /= 10) ++d;
    return d;
}

static bool is_num_pal(uint64_t v) {
    uint64_t rev = 0, x = v;
    for (; x > 0; x /= 10) rev = rev * 10 + x % 10;
    return v == rev;
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
    if (n == 0) n = 1;

    uint64_t lo = n * me / P, hi = n * (me + 1) / P;

    // my chunk's best palindrome
    uint64_t bd = 0, bv = 0;   // digits, value (0 digits = none)
    for (uint64_t i = lo; i < hi; ++i) {
        uint64_t v = value_at(i);
        if (!is_num_pal(v)) continue;
        uint64_t d = digits_of(v);
        if (d > bd || (d == bd && v > bv)) { bd = d; bv = v; }
    }

    // rank 0 hosts P slots of (digits, value)
    BCL::GlobalPtr<uint64_t> parts = nullptr;
    if (me == 0) {
        parts = BCL::alloc<uint64_t>(2 * P);
        for (uint64_t r = 0; r < 2 * P; ++r) parts.local()[r] = 0;
    }
    parts = BCL::broadcast(parts, 0);

    bclx::aput_sync(bd, parts + 2 * me);
    bclx::aput_sync(bv, parts + 2 * me + 1);
    bclx::barrier_sync();

    if (me == 0) {
        uint64_t gd = 0, gv = 0;
        for (uint64_t r = 0; r < P; ++r) {
            uint64_t d = parts.local()[2 * r], v = parts.local()[2 * r + 1];
            if (d > gd || (d == gd && v > gv)) { gd = d; gv = v; }
        }
        if (gd > 0)
            printf("longest palindrome = %llu (%llu digits, chunk-reduce over %llu ranks)\n",
                   gv, gd, P);
        else
            printf("no palindromic element in the array\n");
        BCL::dealloc<uint64_t>(parts);
    }

    BCL::finalize();
    return 0;
}
