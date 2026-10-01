// 104 — Finding Circular Rotation of an Array by K Positions | v1: brute force
// B is constructed deterministically as A rotated left by some K; the
// program finds every K that turns A into B (there is at least one).
// Run: make run PROB=104_circular_rotation_check SRC=result/v1_basic.cpp NP=4 ARGS="10 3"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n> <hidden K>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0, k = 0;
    if (BCL::rank() == 0) {
        n = strtoull(argv[1], nullptr, 10);
        k = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    k = BCL::broadcast(k, 0);
    if (n == 0) n = 1;
    k %= n;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n), b(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);
        for (uint64_t i = 0; i < n; ++i) b[i] = a[(i + k) % n];   // B = A rotated by k

        printf("B is a circular rotation of A by K in: ");
        uint64_t found = 0;
        for (uint64_t cand = 0; cand < n; ++cand) {
            bool ok = true;
            for (uint64_t i = 0; i < n && ok; ++i)
                if (b[i] != a[(i + cand) % n]) ok = false;
            if (ok) {
                printf("%llu ", cand);
                ++found;
            }
        }
        if (found == 0) printf("(none)");
        printf("(hidden K was %llu)\n", k);
    }

    BCL::finalize();
    return 0;
}
