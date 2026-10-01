// 81 — Sort the Elements of an Array | v1: std::sort
// Run: make run PROB=81_sort_array SRC=result/v1_basic.cpp NP=4 ARGS="16"

#include <cstdio>
#include <cstdlib>
#include <vector>
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

    std::vector<uint64_t> a(n);
    for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

    std::sort(a.begin(), a.end());

    if (BCL::rank() == 0) {
        printf("sorted: ");
        for (uint64_t i = 0; i < n && i < 30; ++i) printf("%llu ", a[i]);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
