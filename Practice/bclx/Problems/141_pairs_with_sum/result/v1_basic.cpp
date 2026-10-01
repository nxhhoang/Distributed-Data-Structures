// 141 — Find All Pairs With a Sum Equal to a Given Number | v1: hash count
// One pass with a multiset-style count map: for each element, pairs formed
// with previously seen complements.
// Run: make run PROB=141_pairs_with_sum SRC=result/v1_basic.cpp NP=4 ARGS="16 70"

#include <cstdio>
#include <cstdlib>
#include <map>
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

    uint64_t n = 0, target = 0;
    if (BCL::rank() == 0) {
        n = strtoull(argv[1], nullptr, 10);
        target = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    target = BCL::broadcast(target, 0);
    if (n == 0) n = 1;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

        std::map<uint64_t, uint64_t> seen;
        uint64_t pairs = 0;
        for (uint64_t i = 0; i < n; ++i) {
            // complement already seen?
            auto it = seen.find(target - a[i]);
            if (it != seen.end()) pairs += it->second;
            ++seen[a[i]];
        }

        printf("pairs summing to %llu: %llu\n", target, pairs);
    }

    BCL::finalize();
    return 0;
}
