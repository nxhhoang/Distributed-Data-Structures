// 74 — Find the Largest Element in an Array | v1: single scan
// (57/v2 has the distributed-chunk version with the recursion.)
// Run: make run PROB=74_largest_element SRC=result/v1_basic.cpp NP=4 ARGS="16"

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

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n == 0) n = 1;

    std::vector<uint64_t> a(n);
    for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

    uint64_t m = a[0];
    for (uint64_t i = 1; i < n; ++i)
        if (a[i] > m) m = a[i];

    if (BCL::rank() == 0)
        printf("largest of %llu elements = %llu\n", n, m);

    BCL::finalize();
    return 0;
}
