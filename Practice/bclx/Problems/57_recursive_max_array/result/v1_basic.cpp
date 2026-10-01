// 57 — Largest Element in an Array (recursion) | v1: recursion on rank 0
// max(a, i) = max(a[i], max(a, i-1)). The array is generated deterministically
// from the index, like problem 04.
// Run: make run PROB=57_recursive_max_array SRC=result/v1_basic.cpp NP=4 ARGS="16"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 1000;
}

static uint64_t max_rec(const std::vector<uint64_t> &a, size_t i) {
    if (i == 0) return a[0];
    uint64_t m = max_rec(a, i - 1);
    return a[i] > m ? a[i] : m;
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
        printf("largest of %llu elements = %llu (recursion)\n", n, max_rec(a, n - 1));

    BCL::finalize();
    return 0;
}
