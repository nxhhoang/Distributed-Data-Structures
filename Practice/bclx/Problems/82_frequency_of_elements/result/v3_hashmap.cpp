// 82 — Frequency of Elements | v3: BCL::HashMap (the distributed hash table)
// Upgrade path 2: the counting moves from a local std::map to the library's
// distributed HashMap. Race-free pattern (modify-on-fresh reads uninitialized
// memory, so fresh entries are avoided):
//   A) each rank counts its chunk LOCALLY (std::map)
//   B) insert_or_assign(v, 0) for its keys — idempotent pre-seed
//   barrier
//   C) modify(v, +local_count) — atomic RMW; entries exist by now
//   barrier, then every rank reports its LOCAL segment of the table.
// Run: make run PROB=82_frequency_of_elements SRC=result/v3_hashmap.cpp NP=4 ARGS="60"

#include <cstdio>
#include <cstdlib>
#include <map>
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

    BCL::HashMap<uint64_t, uint64_t> map(1024);   // collective ctor

    uint64_t lo = n * me / P, hi = n * (me + 1) / P;

    // A) local counting over my chunk
    std::map<uint64_t, uint64_t> local;
    for (uint64_t i = lo; i < hi; ++i) ++local[value_at(i)];

    // B) idempotent pre-seed of my keys (value 0)
    for (const auto &kv : local)
        map.insert_or_assign(kv.first, 0);
    bclx::barrier_sync();

    // C) atomic add of my counts — entries all exist now
    for (const auto &kv : local)
        map.modify(kv.first, [kv](uint64_t c) { return c + kv.second; });

    bclx::barrier_sync();

    // D) distributed iteration: my local segment of the global table
    uint64_t total = 0;
    printf("[rank %llu]", me);
    for (auto it = map.local_begin(); it != map.local_end(); ++it) {
        std::pair<const uint64_t, uint64_t> kv = *it;   // *it is a reference; convert explicitly
        printf(" %llu->%llu", kv.first, kv.second);
        total += kv.second;
    }
    printf("\n");

    uint64_t all = bclx::allreduce(total, BCL::sum<uint64_t>{});
    if (me == 0)
        printf("total counted = %llu (expected %llu -> %s)\n",
               all, n, all == n ? "MATCH" : "MISMATCH!");

    BCL::finalize();
    return 0;
}
