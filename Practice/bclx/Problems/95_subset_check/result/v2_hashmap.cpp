// 95 — Determine Whether an Array Is a Subset of Another | v2: BCL::HashMap
// B is built from A (b[i] = a[(7i) mod n]) so it IS a subset; the map holds
// A, and every rank probes its share of B — any miss breaks the subset
// property (first miss reported with fao + aput).
// Run: make run PROB=95_subset_check SRC=result/v2_hashmap.cpp NP=4 ARGS="30 10"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>
#include <bcl/containers/HashMap.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
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

    for (uint64_t i = me; i < n; i += P)
        map.insert_or_assign(value_at(i), 1);

    bclx::barrier_sync();

    BCL::GlobalPtr<uint64_t> missed = nullptr, missing = nullptr;
    if (me == 0) {
        missed = BCL::alloc<uint64_t>(1);
        missing = BCL::alloc<uint64_t>(1);
        *missed.local() = 0;
    }
    missed = BCL::broadcast(missed, 0);
    missing = BCL::broadcast(missing, 0);

    for (uint64_t i = me; i < m; i += P) {
        uint64_t v = value_at((7 * i) % n);   // B drawn from A
        if (map.find(v) == map.end()) {
            if (bclx::fao_sync(missed, uint64_t(1), BCL::plus<uint64_t>{}) == 0)
                bclx::aput_sync(v, missing);
            break;
        }
    }

    bclx::barrier_sync();

    if (me == 0) {
        if (missed.local()[0] > 0)
            printf("B is NOT a subset of A (missing element: %llu)\n",
                   missing.local()[0]);
        else
            printf("B is a subset of A (BCL::HashMap containment)\n");
        BCL::dealloc<uint64_t>(missed);
        BCL::dealloc<uint64_t>(missing);
    }

    BCL::finalize();
    return 0;
}
