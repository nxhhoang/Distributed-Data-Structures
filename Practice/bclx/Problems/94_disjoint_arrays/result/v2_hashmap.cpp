// 94 — Finding Arrays Are Disjoint or Not | v2: BCL::HashMap membership
// A goes into the distributed map (striped inserts); after a barrier every
// rank probes its share of B with find() — the FIRST hit claims the report
// slot with fao (first-reporter-wins, the 45/v2 pattern).
// Run: make run PROB=94_disjoint_arrays SRC=result/v2_hashmap.cpp NP=4 ARGS="40 25"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>
#include <bcl/containers/HashMap.hpp>

static uint64_t value_a(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
}
static uint64_t value_b(uint64_t i) {
    return (i * 40503ull + 7) % 100;
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

    BCL::HashMap<uint64_t, uint64_t> map(1024);

    // A into the map (striped)
    for (uint64_t i = me; i < n; i += P)
        map.insert_or_assign(value_a(i), 1);

    bclx::barrier_sync();

    // first-reporter reporting slot
    BCL::GlobalPtr<uint64_t> found = nullptr, common = nullptr;
    if (me == 0) {
        found = BCL::alloc<uint64_t>(1);
        common = BCL::alloc<uint64_t>(1);
        *found.local() = 0;
        *common.local() = 0;
    }
    found = BCL::broadcast(found, 0);
    common = BCL::broadcast(common, 0);

    // probe my share of B
    for (uint64_t i = me; i < m; i += P) {
        if (map.find(value_b(i)) != map.end()) {
            if (bclx::fao_sync(found, uint64_t(1), BCL::plus<uint64_t>{}) == 0)
                bclx::aput_sync(value_b(i), common);
            break;
        }
    }

    bclx::barrier_sync();

    if (me == 0) {
        if (found.local()[0] > 0)
            printf("arrays are NOT disjoint (common element: %llu, BCL::HashMap lookup)\n",
                   common.local()[0]);
        else
            printf("arrays are disjoint (BCL::HashMap lookups)\n");
        BCL::dealloc<uint64_t>(found);
        BCL::dealloc<uint64_t>(common);
    }

    BCL::finalize();
    return 0;
}
