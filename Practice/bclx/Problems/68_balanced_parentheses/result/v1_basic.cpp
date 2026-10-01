// 68 — Generate All Combinations of Well-Formed Parentheses | v1: backtracking
// At every position: place '(' while open < n, place ')' while close < open.
// Run: make run PROB=68_balanced_parentheses SRC=result/v1_basic.cpp NP=4 ARGS="3"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static void gen(char *buf, int pos, int n, int open, int close) {
    if (pos == 2 * n) {
        buf[pos] = '\0';
        printf("%s\n", buf);
        return;
    }
    if (open < n) {
        buf[pos] = '(';
        gen(buf, pos + 1, n, open + 1, close);
    }
    if (close < open) {
        buf[pos] = ')';
        gen(buf, pos + 1, n, open, close + 1);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <pairs>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n > 8) n = 8;

    if (BCL::rank() == 0) {
        char buf[32];
        gen(buf, 0, (int)n, 0, 0);
    }

    BCL::finalize();
    return 0;
}
