// 100 — Finding the Equilibrium Index of an Array | v1: prefix/suffix sums
// Index i is an equilibrium when sum(a[0..i)) == sum(a[i+1..n)). One pass
// with a running left sum and the total as the right-sum base.
// Run: make run PROB=100_equilibrium_index SRC=result/v1_basic.cpp NP=4 ARGS="12"

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
    if (n == 0) n = 1;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n);
        uint64_t total = 0;
        for (uint64_t i = 0; i < n; ++i) { a[i] = value_at(i); total += a[i]; }

        printf("equilibrium indices: ");
        uint64_t left = 0, found = 0;
        for (uint64_t i = 0; i < n; ++i) {
            uint64_t right = total - left - a[i];
            if (left == right) {
                printf("%llu ", i);
                ++found;
            }
            left += a[i];
        }
        if (found == 0) printf("(none)");
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
