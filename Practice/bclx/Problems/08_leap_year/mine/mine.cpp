// Practice problem: 08_leap_year
// TODO(you): implement it yourself with BCL CoreX.
// Hints (which primitives to practice): see the table in ../../../README.md
// Build & run:
//   make build PROB=08_leap_year SRC=mine/mine.cpp
//   make run   PROB=08_leap_year SRC=mine/mine.cpp NP=4 ARGS="<input>"
#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

using ll = long long;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <year>\n", argv[0]);
        return 1;
    }

    BCL::init();
    // TO DO
    ll y = 0;
    if (BCL::rank() == 0) y = atoll(argv[1]);
    y = BCL::broadcast(y, 0);

    bool leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    if (BCL::rank() == 0)
        printf("%lld is %sa leap year\n", y, leap ? "" : "NOT ");

    BCL::finalize();
    return 0;
}