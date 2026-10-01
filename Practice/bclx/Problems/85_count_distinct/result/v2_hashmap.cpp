// 85 — Counting Distinct Elements | v2: BCL::HashMap
// The set becomes the distributed HashMap: striped insert_or_assign (value
// markers), then a barrier, then every rank counts its LOCAL segment of the
// table — the sum over segments IS the number of distinct values.
// Run: make run PROB=85_count_distinct SRC=result/v2_hashmap.cpp NP=4 ARGS="60"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>
#include <bcl/containers/HashMap.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
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

    BCL::HashMap<uint64_t, uint64_t> map(1024);

    // striped inserts (idempotent — concurrent same-key inserts both write 1)
    for (uint64_t i = me; i < n; i += P)
        map.insert_or_assign(value_at(i), 1);

    bclx::barrier_sync();

    // every distinct value occupies exactly one slot somewhere in the table
    uint64_t mine = 0;
    for (auto it = map.local_begin(); it != map.local_end(); ++it)
        ++mine;

    uint64_t distinct = bclx::allreduce(mine, BCL::sum<uint64_t>{});
    if (me == 0)
        printf("distinct elements among %llu = %llu (BCL::HashMap segments)\n",
               n, distinct);

    BCL::finalize();
    return 0;
}
