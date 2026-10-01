// 150 — Trapping Rain Water Problem | v1: two pointers
// The water above a bar is min(maxLeft, maxRight) - height. The two-pointer
// walk keeps running maxima from both ends and always advances the smaller
// side — that side's water is already determined.
// Run: make run PROB=150_trapping_rain SRC=result/v1_basic.cpp NP=4 ARGS="12"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 10;   // bar heights 0..9
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
        std::vector<uint64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

        uint64_t l = 0, r = n - 1, lm = 0, rm = 0;
        uint64_t water = 0;
        while (l <= r) {
            if (a[l] <= a[r]) {
                if (a[l] >= lm) lm = a[l];
                else water += lm - a[l];
                ++l;
            } else {
                if (a[r] >= rm) rm = a[r];
                else water += rm - a[r];
                --r;
            }
        }

        printf("heights: ");
        for (uint64_t i = 0; i < n && i < 30; ++i) printf("%llu ", a[i]);
        printf("\ntrapped water = %llu units\n", water);
    }

    BCL::finalize();
    return 0;
}
