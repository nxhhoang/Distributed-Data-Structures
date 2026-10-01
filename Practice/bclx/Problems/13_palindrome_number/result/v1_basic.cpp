// 13 — Palindrome Number | v1: broadcast, compare against reverse
// Run: make run PROB=13_palindrome_number SRC=result/v1_basic.cpp NP=4 ARGS="12321"

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

    uint64_t rev = 0;
    for (uint64_t x = n; x > 0; x /= 10) rev = rev * 10 + x % 10;
    bool pal = (rev == n);

    if (BCL::rank() == 0)
        printf("%llu is %sa palindrome\n", n, pal ? "" : "NOT ");

    BCL::finalize();
    return 0;
}
