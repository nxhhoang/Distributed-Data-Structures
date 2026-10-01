// 92 — Find All Symmetric Pairs in an Array | v1: nested scan
// A pair (a, b) is symmetric when (b, a) also appears in the set of pairs.
// The pair array is constructed so that pairs 4k+1 mirror pairs 4k — half of
// them are symmetric by design, plus whatever occurs by chance.
// Run: make run PROB=92_symmetric_pairs SRC=result/v1_basic.cpp NP=4 ARGS="12"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <number of pairs>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t m = 0;
    if (BCL::rank() == 0) m = strtoull(argv[1], nullptr, 10);
    m = BCL::broadcast(m, 0);
    if (m < 2) m = 2;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> x(m), y(m);
        for (uint64_t i = 0; i < m; ++i) {
            x[i] = value_at(i);
            y[i] = value_at(i + 7777);
        }
        for (uint64_t i = 1; i < m; i += 4) {   // make every 4k+1 pair mirror pair 4k
            x[i] = y[i - 1];
            y[i] = x[i - 1];
        }

        printf("symmetric pairs:\n");
        for (uint64_t i = 0; i < m; ++i) {
            for (uint64_t j = i + 1; j < m; ++j) {   // j > i: each pair reported once
                if (x[i] == y[j] && y[i] == x[j]) {
                    printf("  (%llu, %llu) <-> (%llu, %llu)\n",
                           x[i], y[i], x[j], y[j]);
                    break;
                }
            }
        }
    }

    BCL::finalize();
    return 0;
}
