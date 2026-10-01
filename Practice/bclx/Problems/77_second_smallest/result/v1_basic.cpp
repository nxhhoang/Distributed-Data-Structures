// 77 — Find the Second Smallest Element in an Array | v1: one pass, two trackers
// Distinct values only: an element equal to the current minimum does not
// update the second-smallest slot.
// Run: make run PROB=77_second_smallest SRC=result/v1_basic.cpp NP=4 ARGS="16"

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
    if (n < 2) n = 2;

    std::vector<uint64_t> a(n);
    for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

    uint64_t s1 = ~0ull, s2 = ~0ull;   // smallest, second smallest
    for (uint64_t i = 0; i < n; ++i) {
        if (a[i] < s1) { s2 = s1; s1 = a[i]; }
        else if (a[i] < s2 && a[i] != s1) s2 = a[i];
    }

    if (BCL::rank() == 0) {
        if (s2 == ~0ull) printf("no second smallest (all elements equal)\n");
        else printf("smallest = %llu, second smallest = %llu\n", s1, s2);
    }

    BCL::finalize();
    return 0;
}
