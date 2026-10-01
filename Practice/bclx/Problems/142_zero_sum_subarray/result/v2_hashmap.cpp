// 142 — Find if There Is Any Subarray With Sum Zero | v2: BCL::HashMap prefix set
// Prefix sums are markers in the distributed HashMap: a repeated prefix (or a
// prefix of 0) means the subarray between them sums to zero. Rank 0 drives
// the pass; the data is built with a guaranteed zero-sum pair (5, -5) at the
// end.
// Run: make run PROB=142_zero_sum_subarray SRC=result/v2_hashmap.cpp NP=4 ARGS="10"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>
#include <bcl/containers/HashMap.hpp>

static int64_t value_signed(uint64_t i) {
    return (int64_t)((i * 2654435761ull + 17) % 200) - 100;   // -100..99
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
    if (n < 3) n = 3;

    // collective: the HashMap constructor broadcasts its segments — EVERY rank
    // must construct it (only rank 0 drives the pass below).
    BCL::HashMap<uint64_t, uint64_t> map(1024);

    if (BCL::rank() == 0) {
        std::vector<int64_t> a(n);
        for (uint64_t i = 0; i < n - 2; ++i) a[i] = value_signed(i);
        a[n - 2] = 5;
        a[n - 1] = -5;

        map.insert_or_assign(0, 1);   // the empty prefix

        bool found = false;
        uint64_t end = 0;
        int64_t prefix = 0;
        for (uint64_t i = 0; i < n && !found; ++i) {
            prefix += a[i];
            uint64_t p = (uint64_t)prefix;   // wrap: markers only need equality
            if (map.find(p) != map.end()) { found = true; end = i; }
            else map.insert_or_assign(p, 1);
        }

        if (found)
            printf("zero-sum subarray exists, ending at index %llu (distributed prefix set)\n", end);
        else
            printf("no zero-sum subarray\n");
    }

    BCL::finalize();
    return 0;
}
