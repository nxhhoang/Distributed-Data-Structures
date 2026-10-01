// 52 — Finding the Number of Times Digit x Occurs in a Given Input | v1: one number
// Run: make run PROB=52_digit_occurrences SRC=result/v1_basic.cpp NP=4 ARGS="122333 3"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n> <digit x>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0, x = 0;
    if (BCL::rank() == 0) {
        n = strtoull(argv[1], nullptr, 10);
        x = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    x = BCL::broadcast(x, 0);

    if (BCL::rank() == 0) {
        if (x > 9) {
            fprintf(stderr, "digit must be 0..9\n");
        } else {
            uint64_t count = 0;
            for (uint64_t v = n; v > 0; v /= 10)
                if (v % 10 == x) ++count;
            printf("digit %llu occurs %llu time%s in %llu\n",
                   x, count, count == 1 ? "" : "s", n);
        }
    }

    BCL::finalize();
    return 0;
}
