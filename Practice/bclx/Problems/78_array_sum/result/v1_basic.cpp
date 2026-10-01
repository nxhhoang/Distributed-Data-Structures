// 78 — Calculate the Sum of Elements in an Array | v1: single loop
// (Problem 04 is the same sum over a partitioned array + allreduce.)
// Run: make run PROB=78_array_sum SRC=result/v1_basic.cpp NP=4 ARGS="16"

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

    uint64_t sum = 0;
    for (uint64_t i = 0; i < n; ++i) sum += value_at(i);

    if (BCL::rank() == 0)
        printf("sum of %llu elements = %llu\n", n, sum);

    BCL::finalize();
    return 0;
}
