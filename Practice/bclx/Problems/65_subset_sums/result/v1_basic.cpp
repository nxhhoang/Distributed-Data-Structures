// 65 — Print Sums of All Subsets | v1: include/exclude recursion on rank 0
// Classic 2^N recursion: each element is either in or out of the current sum.
// Run: make run PROB=65_subset_sums SRC=result/v1_basic.cpp NP=4 ARGS="4"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100 + 1;
}

static void subset_sums(const std::vector<uint64_t> &a, size_t i, uint64_t cur,
                        std::vector<uint64_t> &out) {
    if (i == a.size()) {
        out.push_back(cur);
        return;
    }
    subset_sums(a, i + 1, cur, out);          // exclude a[i]
    subset_sums(a, i + 1, cur + a[i], out);   // include a[i]
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
    if (n > 20) n = 20;   // 2^20 sums is already plenty

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

        std::vector<uint64_t> out;
        subset_sums(a, 0, 0, out);
        std::sort(out.begin(), out.end());

        printf("array: ");
        for (uint64_t v : a) printf("%llu ", v);
        printf("\n%llu subset sums: ", (uint64_t)out.size());
        for (uint64_t s : out) printf("%llu ", s);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
