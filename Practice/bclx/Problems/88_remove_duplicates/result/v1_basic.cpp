// 88 — Removing Duplicate Elements from an Array | v1: seen-set, order preserved
// Run: make run PROB=88_remove_duplicates SRC=result/v1_basic.cpp NP=4 ARGS="30"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
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
        std::set<uint64_t> seen;
        std::vector<uint64_t> out;
        for (uint64_t i = 0; i < n; ++i) {
            uint64_t v = value_at(i);
            if (seen.insert(v).second) out.push_back(v);
        }

        printf("with duplicates removed (order kept): ");
        for (uint64_t v : out) printf("%llu ", v);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
