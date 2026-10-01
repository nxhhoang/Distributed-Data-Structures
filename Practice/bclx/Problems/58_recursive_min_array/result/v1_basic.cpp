// 58 — Smallest Element in an Array (recursion) | v1: mirror of 57/v1
// Run: make run PROB=58_recursive_min_array SRC=result/v1_basic.cpp NP=4 ARGS="16"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 1000;
}

static uint64_t min_rec(const std::vector<uint64_t> &a, size_t i) {
    if (i == 0) return a[0];
    uint64_t m = min_rec(a, i - 1);
    return a[i] < m ? a[i] : m;
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

    if (BCL::rank() == 0)
        printf("smallest of %llu elements = %llu (recursion)\n", n, min_rec(a, n - 1));

    BCL::finalize();
    return 0;
}
