// 149 — Find the Longest Consecutive Subsequence | v2: BCL::HashMap membership
// All values striped into the map; after a barrier, rank me examines the
// candidate run-STARTS v with v % P == me: a start must exist (find) while
// v-1 must NOT (find). The run length is then probed with finds. The local
// best (len, start) pairs are aput-ed to rank 0 (the 76/v2 composite reduce).
// Run: make run PROB=149_longest_consecutive SRC=result/v2_hashmap.cpp NP=4 ARGS="60"

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

    for (uint64_t i = me; i < n; i += P)
        map.insert_or_assign(value_at(i), 1);

    bclx::barrier_sync();

    // my run-starts: v in [0, 100) with v % P == me
    uint64_t best_len = 0, best_start = 0;
    for (uint64_t v = me; v < 100; v += P) {
        if (map.find(v) == map.end()) continue;
        if (v > 0 && map.find(v - 1) != map.end()) continue;   // not a start
        uint64_t len = 1;
        while (v + len < 100 && map.find(v + len) != map.end())
            ++len;
        if (len > best_len) { best_len = len; best_start = v; }
    }

    // composite aput-reduce at rank 0
    BCL::GlobalPtr<uint64_t> parts = nullptr;
    if (me == 0) {
        parts = BCL::alloc<uint64_t>(2 * P);
        for (uint64_t r = 0; r < 2 * P; ++r) parts.local()[r] = 0;
    }
    parts = BCL::broadcast(parts, 0);

    bclx::aput_sync(best_len, parts + 2 * me);
    bclx::aput_sync(best_start, parts + 2 * me + 1);
    bclx::barrier_sync();

    if (me == 0) {
        uint64_t gl = 0, gs = 0;
        for (uint64_t r = 0; r < P; ++r) {
            uint64_t l = parts.local()[2 * r], s = parts.local()[2 * r + 1];
            if (l > gl) { gl = l; gs = s; }
        }
        printf("longest consecutive run: %llu (starting at %llu, BCL::HashMap probing)\n",
               gl, gs);
        BCL::dealloc<uint64_t>(parts);
    }

    BCL::finalize();
    return 0;
}
