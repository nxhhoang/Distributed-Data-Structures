// Practice problem: 01_positive_or_negative
// TODO(you): implement it yourself with BCL CoreX.
// Hints (which primitives to practice): see the table in ../../../README.md
// Build & run:
//   make build PROB=01_positive_or_negative SRC=mine/mine.cpp
//   make run   PROB=01_positive_or_negative SRC=mine/mine.cpp NP=4 ARGS="<input>"
#include<stdio.h>
#include<bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        return 1;
    }

    BCL::init();

    // TODO: your code here
    long long n = 0;
    if (BCL::rank() == 0) n = atoll(argv[1]);
    BCL::broadcast(n, 0);

    if (BCL::rank() == 0) {
        if (n == 0) fprintf(stdout, "%lld is Zero\n", n);
        else if (n > 0) fprintf(stdout, "%lld is Positive\n", n);
        else fprintf(stdout, "%lld is Negative\n", n);
    } 

    BCL::finalize();
    return 0;
}
