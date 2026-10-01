// 59 — Reversing a Number (recursion) | v1: accumulator recursion
// rev(n, acc) = rev(n/10, acc*10 + n%10) — the accumulator carries the digits
// seen so far, so the result is built in the right order.
// Run: make run PROB=59_recursive_reverse SRC=result/v1_basic.cpp NP=4 ARGS="12345"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t rev_rec(uint64_t n, uint64_t acc) {
    return n == 0 ? acc : rev_rec(n / 10, acc * 10 + n % 10);
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

    if (BCL::rank() == 0)
        printf("reverse of %llu = %llu (recursion)\n", n, rev_rec(n, 0));

    BCL::finalize();
    return 0;
}
