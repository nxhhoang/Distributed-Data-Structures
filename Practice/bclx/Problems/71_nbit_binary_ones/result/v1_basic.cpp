// 71 — All N-bit Binary Numbers Having More 1's than or Equal 0's | v1: backtracking
// Every PREFIX must satisfy ones >= zeros, so a '0' may only be appended
// while ones > zeros currently holds.
// Run: make run PROB=71_nbit_binary_ones SRC=result/v1_basic.cpp NP=4 ARGS="3"
// (n=3 -> 111, 110, 101)

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static void gen(char *buf, int pos, int n, int ones, int zeros) {
    if (pos == n) {
        buf[pos] = '\0';
        printf("%s\n", buf);
        return;
    }
    buf[pos] = '1';
    gen(buf, pos + 1, n, ones + 1, zeros);
    if (ones > zeros) {
        buf[pos] = '0';
        gen(buf, pos + 1, n, ones, zeros + 1);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n bits>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n > 20) n = 20;

    if (BCL::rank() == 0) {
        char buf[64];
        gen(buf, 0, (int)n, 0, 0);
    }

    BCL::finalize();
    return 0;
}
