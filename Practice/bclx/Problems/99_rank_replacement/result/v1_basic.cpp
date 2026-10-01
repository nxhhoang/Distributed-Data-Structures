// 99 — Replace Each Element by Its Rank in the Array | v1: sort + index map
// The smallest element gets rank 1; equal elements share the same rank.
// Run: make run PROB=99_rank_replacement SRC=result/v1_basic.cpp NP=4 ARGS="12"

#include <cstdio>
#include <cstdlib>
#include <map>
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

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

        std::vector<uint64_t> sorted(a);
        std::sort(sorted.begin(), sorted.end());

        std::map<uint64_t, uint64_t> rank;
        uint64_t r = 0;
        for (uint64_t v : sorted) {
            if (!rank.count(v)) rank[v] = ++r;   // ties share the rank
        }

        printf("original: ");
        for (uint64_t v : a) printf("%llu ", v);
        printf("\nby rank:  ");
        for (uint64_t v : a) printf("%llu ", rank[v]);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
