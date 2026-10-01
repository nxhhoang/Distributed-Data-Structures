// 131 — Union and Intersection of Two Sorted Arrays | v2: striped bsearch + fao
// Each rank takes a striped subset of A's elements (i % P == me) and checks
// membership in B with binary search; every hit increments rank 0's
// intersection counter with fao. Because both arrays are distinct-valued,
// |union| = n + m - |intersection| — the distributed count yields both sizes.
// Run: make run PROB=131_union_intersection SRC=result/v2_striped_bsearch.cpp NP=4 ARGS="10 12"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_a(uint64_t i) { return 3 * i; }
static uint64_t value_b(uint64_t i) { return 4 * i; }

static bool contains(const std::vector<uint64_t> &b, uint64_t v) {
    uint64_t lo = 0, hi = b.size();
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (b[mid] == v) return true;
        if (b[mid] < v) lo = mid + 1;
        else hi = mid;
    }
    return false;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n A> <m B>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t n = 0, m = 0;
    if (me == 0) {
        n = strtoull(argv[1], nullptr, 10);
        m = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    m = BCL::broadcast(m, 0);
    if (n == 0) n = 1;
    if (m == 0) m = 1;

    BCL::GlobalPtr<uint64_t> inter = nullptr;
    if (me == 0) {
        inter = BCL::alloc<uint64_t>(1);
        *inter.local() = 0;
    }
    inter = BCL::broadcast(inter, 0);

    // every rank regenerates B for its own binary searches (deterministic)
    std::vector<uint64_t> b(m);
    for (uint64_t i = 0; i < m; ++i) b[i] = value_b(i);

    uint64_t my_hits = 0;
    for (uint64_t i = me; i < n; i += P)          // my striped share of A
        if (contains(b, value_a(i))) ++my_hits;

    if (my_hits > 0)
        bclx::fao_sync(inter, my_hits, BCL::plus<uint64_t>{});

    bclx::barrier_sync();

    if (me == 0) {
        printf("intersection size = %llu, union size = %llu (n + m - inter)\n",
               inter.local()[0], n + m - inter.local()[0]);
        BCL::dealloc<uint64_t>(inter);
    }

    BCL::finalize();
    return 0;
}
