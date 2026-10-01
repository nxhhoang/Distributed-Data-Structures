// 91 — Counting Even and Odd Elements in an Array | v1: single loop
// Run: make run PROB=91_count_even_odd SRC=result/v1_basic.cpp NP=4 ARGS="30"

#include <cstdio>
#include <cstdlib>
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

    uint64_t even = 0, odd = 0;
    for (uint64_t i = 0; i < n; ++i) {
        if (value_at(i) % 2 == 0) ++even;
        else ++odd;
    }

    if (BCL::rank() == 0)
        printf("even = %llu, odd = %llu (of %llu)\n", even, odd, n);

    BCL::finalize();
    return 0;
}
