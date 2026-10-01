// 130 — Move All the Negative Elements to One Side of the Array | v1: partition
// One left-to-right pass with a "next negative slot" pointer; relative order
// inside each side is not preserved (quicksort-partition style).
// Run: make run PROB=130_move_negatives SRC=result/v1_basic.cpp NP=4 ARGS="16"

#include <cstdio>
#include <cstdlib>
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
    if (n == 0) n = 1;

    if (BCL::rank() == 0) {
        std::vector<int64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_signed(i);

        uint64_t j = 0;   // next slot for a negative
        for (uint64_t i = 0; i < n; ++i) {
            if (a[i] < 0) {
                int64_t t = a[j]; a[j] = a[i]; a[i] = t;
                ++j;
            }
        }

        printf("negatives first: ");
        for (uint64_t i = 0; i < n && i < 30; ++i) printf("%lld ", a[i]);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
