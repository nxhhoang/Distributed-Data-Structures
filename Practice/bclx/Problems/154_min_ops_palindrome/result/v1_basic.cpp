// 154 — Minimum Number of Operations to Make an Array a Palindrome | v1
// Two pointers: matching ends move inward; otherwise MERGE the smaller side
// into its neighbor (one operation) and keep going.
// Run: make run PROB=154_min_ops_palindrome SRC=result/v1_basic.cpp NP=4 ARGS="10"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 20 + 1;
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

        uint64_t i = 0, j = n - 1, ops = 0;
        while (i < j) {
            if (a[i] == a[j]) {
                ++i;
                if (j == 0) break;
                --j;
            } else if (a[i] < a[j]) {
                a[i + 1] += a[i];   // merge the smaller left into its neighbor
                ++i;
                ++ops;
            } else {
                a[j - 1] += a[j];   // merge the smaller right into its neighbor
                if (j == 0) break;
                --j;
                ++ops;
            }
        }

        printf("minimum merge operations to a palindrome = %llu\n", ops);
    }

    BCL::finalize();
    return 0;
}
