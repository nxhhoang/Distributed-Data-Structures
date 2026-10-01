// 141 — Pairs With a Given Sum | v2: striped outer loop + fao count
// Rank me fixes the outer elements i with i % P == me and counts the pairs
// (i, j), j > i, whose sum matches — no double counting because the outer
// index is partitioned. Each rank's local pair count is fao-ed into rank 0's
// total (batched: ONE fao per rank).
// Run: make run PROB=141_pairs_with_sum SRC=result/v2_striped.cpp NP=4 ARGS="16 70"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n> <target>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t n = 0, target = 0;
    if (me == 0) {
        n = strtoull(argv[1], nullptr, 10);
        target = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    target = BCL::broadcast(target, 0);
    if (n == 0) n = 1;
    if (n > 2000) n = 2000;

    BCL::GlobalPtr<uint64_t> total = nullptr;
    if (me == 0) {
        total = BCL::alloc<uint64_t>(1);
        *total.local() = 0;
    }
    total = BCL::broadcast(total, 0);

    // every rank regenerates the deterministic array
    std::vector<uint64_t> a(n);
    for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

    uint64_t my_pairs = 0;
    for (uint64_t i = me; i < n; i += P)          // striped outer index
        for (uint64_t j = i + 1; j < n; ++j)
            if (a[i] + a[j] == target) ++my_pairs;

    if (my_pairs > 0)
        bclx::fao_sync(total, my_pairs, BCL::plus<uint64_t>{});

    bclx::barrier_sync();

    if (me == 0) {
        printf("pairs summing to %llu: %llu (striped over %llu ranks)\n",
               target, total.local()[0], P);
        BCL::dealloc<uint64_t>(total);
    }

    BCL::finalize();
    return 0;
}
