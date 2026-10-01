// 141 — Find All Pairs With a Given Sum | v3: BCL::HashMap seen-map
// The classic one-pass seen-map, but the map is the library's distributed
// HashMap — every find/modify is a remote RMA on the slot owner. Rank 0
// drives the pass (the map is single-writer here, so find-then-insert is
// race-free; multi-rank counting needs 82/v3's pre-seed pattern).
// Run: make run PROB=141_pairs_with_sum SRC=result/v3_hashmap.cpp NP=4 ARGS="16 70"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>
#include <bcl/containers/HashMap.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n> <target>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0, target = 0;
    if (BCL::rank() == 0) {
        n = strtoull(argv[1], nullptr, 10);
        target = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    target = BCL::broadcast(target, 0);
    if (n == 0) n = 1;

    BCL::HashMap<uint64_t, uint64_t> map(1024);

    if (BCL::rank() == 0) {
        uint64_t pairs = 0;
        for (uint64_t i = 0; i < n; ++i) {
            uint64_t v = value_at(i);
            auto it = map.find(target - v);      // complements seen so far
            if (it != map.end())
                pairs += *it;
            // add v to the seen multiset
            auto self = map.find(v);
            if (self == map.end())
                map.insert_or_assign(v, 1);
            else
                map.modify(v, [](uint64_t c) { return c + 1; });
        }
        printf("pairs summing to %llu: %llu (distributed seen-map)\n", target, pairs);
    }

    BCL::finalize();
    return 0;
}
