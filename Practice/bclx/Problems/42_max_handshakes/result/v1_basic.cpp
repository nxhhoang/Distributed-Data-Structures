// 42 — Maximum Number of Handshakes | v1: broadcast + C(n,2)
// Every person shakes hands with every other exactly once: n*(n-1)/2.
// Run: make run PROB=42_max_handshakes SRC=result/v1_basic.cpp NP=4 ARGS="30"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);

    uint64_t h = (n < 2) ? 0 : n * (n - 1) / 2;

    if (BCL::rank() == 0)
        printf("maximum handshakes among %llu people = %llu\n", n, h);

    BCL::finalize();
    return 0;
}
