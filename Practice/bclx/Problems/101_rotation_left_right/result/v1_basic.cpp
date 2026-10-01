// 101 — Rotation of Elements of Array: Left and Right | v1: index arithmetic
// left(d): out[i] = a[(i+d) % n];  right(d): out[i] = a[(i+n-d) % n].
// Run: make run PROB=101_rotation_left_right SRC=result/v1_basic.cpp NP=4 ARGS="10 3"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
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
        printf("original:   ");
        for (uint64_t i = 0; i < n; ++i) printf("%llu ", value_at(i));
        printf("\nleft by %llu:  ", d);
        for (uint64_t i = 0; i < n; ++i) printf("%llu ", value_at((i + d) % n));
        printf("\nright by %llu: ", d);
        for (uint64_t i = 0; i < n; ++i) printf("%llu ", value_at((i + n - d) % n));
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
