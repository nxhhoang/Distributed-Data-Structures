// 142 — Find if There Is Any Subarray With Sum Equal to Zero | v1: prefix sums + set
// A subarray [i+1..j] sums to zero iff two prefix sums are equal. The data is
// constructed with a guaranteed zero-sum pair at the end (5, -5).
// Run: make run PROB=142_zero_sum_subarray SRC=result/v1_basic.cpp NP=4 ARGS="10"

#include <cstdio>
#include <cstdlib>
#include <set>
#include <vector>
#include <bclx/bclx.hpp>

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

    if (BCL::rank() == 0) {
        std::vector<int64_t> a(n);
        for (uint64_t i = 0; i < n - 2; ++i) a[i] = value_signed(i);
        a[n - 2] = 5;     // a guaranteed zero-sum subarray...
        a[n - 1] = -5;    // ...at the very end

        std::set<int64_t> seen;
        int64_t prefix = 0;
        bool found = false;
        uint64_t end = 0;

        seen.insert(0);   // empty prefix
        for (uint64_t i = 0; i < n && !found; ++i) {
            prefix += a[i];
            if (seen.count(prefix)) { found = true; end = i; }
            else seen.insert(prefix);
        }

        if (found)
            printf("zero-sum subarray exists, ending at index %llu\n", end);
        else
            printf("no zero-sum subarray\n");
    }

    BCL::finalize();
    return 0;
}
