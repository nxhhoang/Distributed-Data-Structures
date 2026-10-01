// 146 — Find Elements Appearing More Than n/k Times | v2: BCL::HashMap counting
// The local-count + pre-seed + modify pattern of 82/v3, then every rank
// reports its local-segment entries with count > n/k.
// Run: make run PROB=146_more_than_n_by_k SRC=result/v2_hashmap.cpp NP=4 ARGS="60 4"

#include <cstdio>
#include <cstdlib>
#include <map>
#include <bclx/bclx.hpp>
#include <bcl/containers/HashMap.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 50;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n> <k>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t n = 0, k = 0;
    if (me == 0) {
        n = strtoull(argv[1], nullptr, 10);
        k = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    k = BCL::broadcast(k, 0);
    if (n == 0) n = 1;
    if (k == 0) k = 1;

    BCL::HashMap<uint64_t, uint64_t> map(1024);

    uint64_t lo = n * me / P, hi = n * (me + 1) / P;
    std::map<uint64_t, uint64_t> local;
    for (uint64_t i = lo; i < hi; ++i) ++local[value_at(i)];

    for (const auto &kv : local)
        map.insert_or_assign(kv.first, 0);
    bclx::barrier_sync();
    for (const auto &kv : local)
        map.modify(kv.first, [kv](uint64_t c) { return c + kv.second; });
    bclx::barrier_sync();

    uint64_t threshold = n / k;
    printf("[rank %llu] more than %llu times:", me, threshold);
    for (auto it = map.local_begin(); it != map.local_end(); ++it) {
        std::pair<const uint64_t, uint64_t> kv = *it;   // *it is a reference; convert explicitly
        if (kv.second > threshold)
            printf(" %llu(%llux)", kv.first, kv.second);
    }
    printf("\n");

    BCL::finalize();
    return 0;
}
