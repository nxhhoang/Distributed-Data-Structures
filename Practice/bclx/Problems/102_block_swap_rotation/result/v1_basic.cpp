// 102 — Block Swap Algorithm for Array Rotation | v1: recursive block swap
// Left rotation by d. Idea: split [0..n) into A = [0..d) and B = [d..n);
// repeatedly swap the smaller of A/B with the tail of the other and recurse
// on the remainder:
//   d == n-d : swap A and B — done
//   d <  n-d : swap A with the LAST d elements, recurse on [0..n-d) with d
//   d >  n-d : swap the FIRST n-d elements with [d..n), recurse on [n-d..n)
//              with d' = 2d-n, size d
// Trace by hand once — it is the best way to understand it.
// Run: make run PROB=102_block_swap_rotation SRC=result/v1_basic.cpp NP=4 ARGS="10 3"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
}

static void swap_blocks(uint64_t *a, uint64_t i, uint64_t j, uint64_t d) {
    for (uint64_t k = 0; k < d; ++k) {
        uint64_t t = a[i + k];
        a[i + k] = a[j + k];
        a[j + k] = t;
    }
}

static void left_rotate_rec(uint64_t *a, uint64_t d, uint64_t n) {
    if (d == 0 || d == n) return;
    if (d == n - d) { swap_blocks(a, 0, n - d, d); return; }
    if (d < n - d) {
        swap_blocks(a, 0, n - d, d);          // A <-> last d elements
        left_rotate_rec(a, d, n - d);         // remainder: [0..n-d) still shifted by d
    } else {
        swap_blocks(a, 0, d, n - d);          // first n-d <-> [d..n)
        left_rotate_rec(a + n - d, 2 * d - n, d);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n> <d>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0, d = 0;
    if (BCL::rank() == 0) {
        n = strtoull(argv[1], nullptr, 10);
        d = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    d = BCL::broadcast(d, 0);
    if (n == 0) n = 1;
    d %= n;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

        left_rotate_rec(a.data(), d, n);

        printf("left-rotated by %llu (block swap): ", d);
        for (uint64_t i = 0; i < n; ++i) printf("%llu ", a[i]);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
