// 88 — Removing Duplicate Elements (order preserved) | v2: BCL::HashMap
// The map stores value -> FIRST index. Sentinel ~0ull is never a real index:
//   A) pre-seed my values with the sentinel (idempotent)
//   barrier
//   B) modify(v, min with my first occurrence) — atomic on existing entries
//   barrier
//   C) rank 0 iterates the WHOLE table (begin/end), sorts by first index,
//      prints the deduplicated order.
// Run: make run PROB=88_remove_duplicates SRC=result/v2_hashmap.cpp NP=4 ARGS="40"

#include <cstdio>
#include <cstdlib>
#include <map>
#include <vector>
#include <algorithm>
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

    // A) my chunk's first occurrence of each value
    uint64_t lo = n * me / P, hi = n * (me + 1) / P;
    std::map<uint64_t, uint64_t> first_idx;
    for (uint64_t i = lo; i < hi; ++i) {
        uint64_t v = value_at(i);
        if (!first_idx.count(v)) first_idx[v] = i;
    }

    const uint64_t SENTINEL = ~0ull;
    for (const auto &kv : first_idx)
        map.insert_or_assign(kv.first, SENTINEL);
    bclx::barrier_sync();

    // B) atomic min-merge of my first indices
    for (const auto &kv : first_idx)
        map.modify(kv.first, [kv](uint64_t old) { return old > kv.second ? kv.second : old; });

    bclx::barrier_sync();

    // C) rank 0: global iteration, restore the order
    if (me == 0) {
        std::vector<std::pair<uint64_t, uint64_t>> out;   // (first index, value)
        for (auto it = map.begin(); it != map.end(); ++it) {
            std::pair<const uint64_t, uint64_t> kv = *it;   // *it is a reference; convert explicitly
            out.push_back({kv.second, kv.first});
        }
        std::sort(out.begin(), out.end());

        printf("duplicates removed (order kept): ");
        for (const auto &p : out) printf("%llu ", p.second);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
