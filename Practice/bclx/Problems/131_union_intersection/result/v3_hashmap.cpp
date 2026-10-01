// 131 — Union and Intersection of Two Sorted Arrays | v3: BCL::HashMap
// A striped into the map; after a barrier each rank probes its share of B:
// a hit is an intersection (fao count), a miss joins the union (insert).
// The union size is the number of occupied slots, counted per local segment
// — and cross-checked against n + m - intersection (distinct arrays).
// Run: make run PROB=131_union_intersection SRC=result/v3_hashmap.cpp NP=4 ARGS="10 12"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>
#include <bcl/containers/HashMap.hpp>

static uint64_t value_a(uint64_t i) { return 3 * i; }
static uint64_t value_b(uint64_t i) { return 4 * i; }

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

    BCL::HashMap<uint64_t, uint64_t> map(1024);

    // A striped into the map
    for (uint64_t i = me; i < n; i += P)
        map.insert_or_assign(value_a(i), 1);

    bclx::barrier_sync();

    // intersection counter at rank 0
    BCL::GlobalPtr<uint64_t> inter = nullptr;
    if (me == 0) {
        inter = BCL::alloc<uint64_t>(1);
        *inter.local() = 0;
    }
    inter = BCL::broadcast(inter, 0);

    // probe my share of B: hit -> intersection, miss -> join the union
    uint64_t my_inter = 0;
    for (uint64_t i = me; i < m; i += P) {
        if (map.find(value_b(i)) != map.end())
            ++my_inter;
        else
            map.insert_or_assign(value_b(i), 1);
    }
    if (my_inter > 0)
        bclx::fao_sync(inter, my_inter, BCL::plus<uint64_t>{});

    bclx::barrier_sync();

    // union size = occupied slots across all local segments
    uint64_t mine = 0;
    for (auto it = map.local_begin(); it != map.local_end(); ++it)
        ++mine;
    uint64_t un = bclx::allreduce(mine, BCL::sum<uint64_t>{});

    if (me == 0) {
        printf("intersection size = %llu, union size = %llu "
               "(check n+m-inter = %llu -> %s)\n",
               inter.local()[0], un, n + m - inter.local()[0],
               un == n + m - inter.local()[0] ? "MATCH" : "MISMATCH!");
        BCL::dealloc<uint64_t>(inter);
    }

    BCL::finalize();
    return 0;
}
