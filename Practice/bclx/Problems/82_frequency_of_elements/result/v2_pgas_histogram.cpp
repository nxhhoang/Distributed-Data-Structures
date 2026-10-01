// 82 — Frequency of Elements | v2: LOCAL histogram per chunk + batched fao
// The distributed histogram: since values are in [0, 100), rank 0 hosts a
// 100-word frequency table. Each rank first counts its OWN chunk into a
// LOCAL table (cheap), then pushes the whole local table into the global one
// with ONE fao per non-empty bucket — batching the remote updates instead
// of paying one fao per element. This local-then-merge shape is the standard
// way to build distributed histograms.
// Run: make run PROB=82_frequency_of_elements SRC=result/v2_pgas_histogram.cpp NP=4 ARGS="30"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

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

    BCL::GlobalPtr<uint64_t> table = nullptr;
    if (me == 0) {
        table = BCL::alloc<uint64_t>(100);
        for (int v = 0; v < 100; ++v) table.local()[v] = 0;
    }
    table = BCL::broadcast(table, 0);

    uint64_t lo = n * me / P, hi = n * (me + 1) / P;

    std::vector<uint64_t> local(100, 0);   // my chunk's histogram
    for (uint64_t i = lo; i < hi; ++i) ++local[value_at(i)];

    // merge: one fao per non-empty bucket (not per element!)
    for (int v = 0; v < 100; ++v)
        if (local[v] > 0)
            bclx::fao_sync(table + v, local[v], BCL::plus<uint64_t>{});

    bclx::barrier_sync();

    if (me == 0) {
        uint64_t total = 0;
        printf("frequencies over %llu elements (distributed histogram):\n", n);
        for (int v = 0; v < 100; ++v) {
            if (table.local()[v] > 0) {
                printf("  %d -> %llu\n", v, table.local()[v]);
                total += table.local()[v];
            }
        }
        printf("total counted = %llu (expected %llu -> %s)\n",
               total, n, total == n ? "MATCH" : "MISMATCH!");
        BCL::dealloc<uint64_t>(table);
    }

    BCL::finalize();
    return 0;
}
