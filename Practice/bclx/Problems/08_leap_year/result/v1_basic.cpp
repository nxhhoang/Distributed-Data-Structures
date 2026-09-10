// 08 — Leap Year or Not | v1: broadcast + rule check
// Rule: divisible by 4 and not by 100, or divisible by 400.
// Run: make run PROB=08_leap_year SRC=result/v1_basic.cpp NP=4 ARGS="2024"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <year>\n", argv[0]);
        return 1;
    }

    BCL::init();

    long long y = 0;
    if (BCL::rank() == 0) y = atoll(argv[1]);
    y = BCL::broadcast(y, 0);

    bool leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    if (BCL::rank() == 0)
        printf("%lld is %sa leap year\n", y, leap ? "" : "NOT ");

    BCL::finalize();
    return 0;
}
