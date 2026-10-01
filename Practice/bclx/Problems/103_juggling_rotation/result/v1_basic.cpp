// 103 — Juggling Algorithm for Array Rotation | v1: gcd cycles
// Left rotation by d moves elements along d cycles, each of length n/gcd(n,d)
// (cycle: i -> i+d mod n). One temp per cycle; total moves = n - gcd(n,d).
// Run: make run PROB=103_juggling_rotation SRC=result/v1_basic.cpp NP=4 ARGS="10 3"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
}

static uint64_t gcd(uint64_t a, uint64_t b) {
    while (b != 0) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static void juggling_left(uint64_t *a, uint64_t n, uint64_t d) {
    uint64_t g = gcd(d, n);
    for (uint64_t i = 0; i < g; ++i) {
        uint64_t tmp = a[i];
        uint64_t j = i;
        while (true) {
            uint64_t k = j + d;
            if (k >= n) k -= n;
            if (k == i) break;
            a[j] = a[k];   // pull the successor back along the cycle
            j = k;
        }
        a[j] = tmp;
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

        juggling_left(a.data(), n, d);

        printf("left-rotated by %llu (juggling, %llu cycles): ", d, gcd(d, n));
        for (uint64_t i = 0; i < n; ++i) printf("%llu ", a[i]);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
