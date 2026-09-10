// Practice problem: 02_even_or_odd
// TODO(you): implement it yourself with BCL CoreX.
// Hints (which primitives to practice): see the table in ../../../README.md
// Build & run:
//   make build PROB=02_even_or_odd SRC=mine/mine.cpp
//   make run   PROB=02_even_or_odd SRC=mine/mine.cpp NP=4 ARGS="<input>"
#include <bclx/bclx.hpp>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        return 1;
    }

    BCL::init();

    int sum = 0;
    int n = atoll(argv[1]);
    int val = n % 2;

    sum = BCL::reduce(val, 
        [&](int x, int y) -> int {return x + y;}, 
        0);

    // TODO: your code here
    if (BCL::rank() == 0) {
        if (sum == BCL::nprocs() && n % 2 == 1) 
            fprintf(stdout, "%lld is Odd\n", n);
        else
            fprintf(stdout, "%lld is Even\n", n);
    }

    BCL::finalize();
    return 0;
}
