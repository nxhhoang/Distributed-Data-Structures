// Practice problem: 07_greatest_of_three
// TODO(you): implement it yourself with BCL CoreX.
// Hints (which primitives to practice): see the table in ../../../README.md
// Build & run:
//   make build PROB=07_greatest_of_three SRC=mine/mine.cpp
//   make run   PROB=07_greatest_of_three SRC=mine/mine.cpp NP=4 ARGS="<input>"
#include <bclx/bclx.hpp>
#include <stdio.h>
#include <stdlib.h>
using ll = long long;

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "usage %s <a> <b> <c>\n", argv[0]);
        return 1;
    }

    BCL::init();

    ll a, b, c;
    if (BCL::rank() == 0) {
        a = atoll(argv[1]);
        b = atoll(argv[2]);
        c = atoll(argv[3]);
    }

    a = BCL::broadcast(a, 0);
    b = BCL::broadcast(b, 0);
    c = BCL::broadcast(c, 0);

    ll maxV = std::max({a, b, c});
    ll d = BCL::reduce(maxV, [](ll a, ll b) -> ll {
        return std::max(a, b);  
    }, 0);

    if (BCL::rank() == 0) {

fprintf(stdout, "greatest(%lld, %lld, %lld) = %lld\n", a, b, c, d);;
        if (BCL::nprocs() == d) {
            fprintf(stdout, "ok\n");
        }
    }

    // TODO: your code here

    BCL::finalize();
    return 0;
}
