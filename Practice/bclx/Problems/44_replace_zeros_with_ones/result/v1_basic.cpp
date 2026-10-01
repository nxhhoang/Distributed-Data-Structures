// 44 — Replace All 0's with 1 in a Given Integer | v1: place-value rebuild
// e.g. 1005 -> 1115. Digits are processed right-to-left with a running
// power of ten so the digit order is preserved.
// Run: make run PROB=44_replace_zeros_with_ones SRC=result/v1_basic.cpp NP=4 ARGS="1005"

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

    uint64_t result = 0, m = 1;
    for (uint64_t x = n; x > 0; x /= 10) {
        uint64_t d = x % 10;
        result += (d == 0 ? 1 : d) * m;
        m *= 10;
    }
    if (n == 0) result = 1;   // "0" has one digit, which becomes 1

    if (BCL::rank() == 0)
        printf("%llu with 0's replaced by 1's = %llu\n", n, result);

    BCL::finalize();
    return 0;
}
