// 95 — Determine Whether an Array Is a Subset of Another | v1: set containment
// B is built FROM A (b[i] = a[(7i) mod n]) so it is a subset by construction;
// change the formula to break it.
// Run: make run PROB=95_subset_check SRC=result/v1_basic.cpp NP=4 ARGS="20 8"

#include <cstdio>
#include <cstdlib>
#include <set>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n A> <m B>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0, m = 0;
    if (BCL::rank() == 0) {
        n = strtoull(argv[1], nullptr, 10);
        m = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    m = BCL::broadcast(m, 0);
    if (n == 0) n = 1;
    if (m == 0) m = 1;
    if (m > n) m = n;

    if (BCL::rank() == 0) {
        std::set<uint64_t> s;
        for (uint64_t i = 0; i < n; ++i) s.insert(value_at(i));

        bool subset = true;
        uint64_t missing = 0;
        for (uint64_t i = 0; i < m; ++i) {
            uint64_t v = value_at((7 * i) % n);   // drawn from A -> subset
            if (!s.count(v)) { subset = false; missing = v; break; }
        }

        if (subset)
            printf("B is a subset of A\n");
        else
            printf("B is NOT a subset of A (missing element: %llu)\n", missing);
    }

    BCL::finalize();
    return 0;
}
