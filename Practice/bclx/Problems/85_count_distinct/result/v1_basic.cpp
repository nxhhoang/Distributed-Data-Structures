// 85 — Counting Distinct Elements in an Array | v1: set
// Run: make run PROB=85_count_distinct SRC=result/v1_basic.cpp NP=4 ARGS="30"

#include <cstdio>
#include <cstdlib>
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
        std::set<uint64_t> s;
        for (uint64_t i = 0; i < n; ++i) s.insert(value_at(i));
        printf("distinct elements among %llu = %zu\n", n, s.size());
    }

    BCL::finalize();
    return 0;
}
