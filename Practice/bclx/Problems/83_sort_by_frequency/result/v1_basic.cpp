// 83 — Sorting Elements of an Array by Frequency | v1: freq map + stable sort
// Higher frequency first; equal frequencies ordered by value ascending.
// Run: make run PROB=83_sort_by_frequency SRC=result/v1_basic.cpp NP=4 ARGS="30"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>
#include <algorithm>
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

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n == 0) n = 1;

    if (BCL::rank() == 0) {
        std::map<uint64_t, uint64_t> freq;
        for (uint64_t i = 0; i < n; ++i) ++freq[value_at(i)];

        std::vector<uint64_t> vals;
        for (const auto &kv : freq) vals.push_back(kv.first);

        std::sort(vals.begin(), vals.end(), [&](uint64_t a, uint64_t b) {
            if (freq[a] != freq[b]) return freq[a] > freq[b];   // frequency descending
            return a < b;                                       // value ascending
        });

        printf("sorted by frequency: ");
        for (uint64_t v : vals) printf("%llu(%llux) ", v, freq[v]);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
