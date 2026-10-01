// 148 — Next Permutation | v1: pivot + swap + reverse suffix
// The lexicographically next arrangement: find the rightmost ascent a[i] <
// a[i+1], swap a[i] with the smallest larger element after it, then reverse
// the suffix. This single step is the engine that 127's loop would use to
// enumerate permutations one by one.
// Run: make run PROB=148_next_permutation SRC=result/v1_basic.cpp NP=4 ARGS="1 3 5 4 2"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <digits...>   e.g. 1 3 5 4 2\n", argv[0]);
        return 1;
    }

    BCL::init();

    std::vector<uint64_t> a(argc - 1);
    for (int i = 1; i < argc; ++i) a[i - 1] = strtoull(argv[i], nullptr, 10);
    uint64_t n = a.size();

    // broadcast the size; every rank SPMD-computes for the exercise
    n = BCL::broadcast(n, 0);
    for (uint64_t i = 0; i < n; ++i)
        a[i] = BCL::broadcast(a[i], 0);

    if (BCL::rank() == 0) {
        printf("input:    ");
        for (uint64_t v : a) printf("%llu ", v);

        // 1. rightmost ascent
        int64_t i = (int64_t)n - 2;
        while (i >= 0 && a[i] >= a[i + 1]) --i;

        if (i >= 0) {
            // 2. smallest element greater than a[i] in the suffix
            int64_t j = (int64_t)n - 1;
            while (a[j] <= a[i]) --j;
            std::swap(a[i], a[j]);
        }
        // 3. reverse the suffix (also handles the "last permutation" wrap)
        std::reverse(a.begin() + i + 1, a.end());

        printf("\nnext:     ");
        for (uint64_t v : a) printf("%llu ", v);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
