// 72 — Find All Subsets of a Set | v1: include/exclude recursion
// The same recursion as 65/v1 (subset sums) but printing the subsets
// themselves; 65/v2 shows the striped-parallel variant of this tree.
// Run: make run PROB=72_all_subsets SRC=result/v1_basic.cpp NP=4 ARGS="3"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100 + 1;
}

static void subsets(const std::vector<uint64_t> &a, size_t i,
                    std::vector<uint64_t> &cur) {
    if (i == a.size()) {
        printf("{ ");
        for (uint64_t v : cur) printf("%llu ", v);
        printf("}\n");
        return;
    }
    subsets(a, i + 1, cur);         // exclude a[i]
    cur.push_back(a[i]);
    subsets(a, i + 1, cur);         // include a[i]
    cur.pop_back();                 // backtrack
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <N>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n > 15) n = 15;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

        printf("set: ");
        for (uint64_t v : a) printf("%llu ", v);
        printf("\nsubsets:\n");

        std::vector<uint64_t> cur;
        subsets(a, 0, cur);
    }

    BCL::finalize();
    return 0;
}
