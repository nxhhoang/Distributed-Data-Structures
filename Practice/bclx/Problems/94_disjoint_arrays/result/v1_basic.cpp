// 94 — Finding Whether Arrays Are Disjoint or Not | v1: set intersection
// A generated with value_at (values < 100), B with a second multiplier —
// whether they collide is deterministic; the first common element is shown.
// Run: make run PROB=94_disjoint_arrays SRC=result/v1_basic.cpp NP=4 ARGS="40 25"

#include <cstdio>
#include <cstdlib>
#include <set>
#include <bclx/bclx.hpp>

static uint64_t value_a(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
}
static uint64_t value_b(uint64_t i) {
    return (i * 40503ull + 7) % 100;
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

    if (BCL::rank() == 0) {
        std::set<uint64_t> s;
        for (uint64_t i = 0; i < n; ++i) s.insert(value_a(i));

        bool disjoint = true;
        uint64_t common = 0;
        for (uint64_t i = 0; i < m && disjoint; ++i) {
            if (s.count(value_b(i))) {
                disjoint = false;
                common = value_b(i);
            }
        }

        if (disjoint)
            printf("arrays are disjoint\n");
        else
            printf("arrays are NOT disjoint (common element: %llu)\n", common);
    }

    BCL::finalize();
    return 0;
}
