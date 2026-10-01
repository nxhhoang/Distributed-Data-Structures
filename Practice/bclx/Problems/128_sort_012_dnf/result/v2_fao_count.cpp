// 128 — Sort an Array of 0s, 1s and 2s | v2: fao histogram + rebuild
// With only three distinct values, "sorting" IS counting: every rank counts
// the 0/1/2 occurrences of its chunk locally, merges the three counts into
// rank 0's fao counters (one fao per value per rank), and rank 0 rebuilds
// the sorted array from the totals. The same distributed-histogram idea as
// 82/v2 and 120/v2, reduced to three buckets.
// Run: make run PROB=128_sort_012_dnf SRC=result/v2_fao_count.cpp NP=4 ARGS="16"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 3;
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

    BCL::GlobalPtr<uint64_t> cnt = nullptr;
    if (me == 0) {
        cnt = BCL::alloc<uint64_t>(3);
        cnt.local()[0] = cnt.local()[1] = cnt.local()[2] = 0;
    }
    cnt = BCL::broadcast(cnt, 0);

    // local counts over my contiguous chunk, then ONE fao per value
    uint64_t lo = n * me / P, hi = n * (me + 1) / P;
    uint64_t local[3] = {0, 0, 0};
    for (uint64_t i = lo; i < hi; ++i) ++local[value_at(i)];
    for (int v = 0; v < 3; ++v)
        if (local[v] > 0)
            bclx::fao_sync(cnt + v, local[v], BCL::plus<uint64_t>{});

    bclx::barrier_sync();

    if (me == 0) {
        uint64_t total = cnt.local()[0] + cnt.local()[1] + cnt.local()[2];
        printf("sorted: ");
        for (int v = 0; v < 3; ++v)
            for (uint64_t k = 0; k < cnt.local()[v] && k + total <= 60; ++k)
                printf("%d ", v);
        printf("(0s=%llu 1s=%llu 2s=%llu, total %llu -> %s)\n",
               cnt.local()[0], cnt.local()[1], cnt.local()[2], total,
               total == n ? "MATCH" : "MISMATCH!");
        BCL::dealloc<uint64_t>(cnt);
    }

    BCL::finalize();
    return 0;
}
