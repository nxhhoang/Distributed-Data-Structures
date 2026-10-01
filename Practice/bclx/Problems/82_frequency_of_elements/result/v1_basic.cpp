// 82 — Finding the Frequency of Elements in an Array | v1: map counting
// Values are generated in [0, 100), so a table works too.
// Run: make run PROB=82_frequency_of_elements SRC=result/v1_basic.cpp NP=4 ARGS="30"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>
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

        printf("frequencies over %llu elements:\n", n);
        for (const auto &kv : freq)
            printf("  %llu -> %llu\n", kv.first, kv.second);
    }

    BCL::finalize();
    return 0;
}
