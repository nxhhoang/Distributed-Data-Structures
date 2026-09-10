// Practice problem: 03_sum_of_first_n_natural
// TODO(you): implement it yourself with BCL CoreX.
// Hints (which primitives to practice): see the table in ../../../README.md
// Build & run:
//   make build PROB=03_sum_of_first_n_natural SRC=mine/mine.cpp
//   make run   PROB=03_sum_of_first_n_natural SRC=mine/mine.cpp NP=4 ARGS="<input>"
#include <bclx/bclx.hpp>
#include <stdio.h>
using ll = long long;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        return 1;
    }

    BCL::init();

    ll n = atoll(argv[1]);
    int num = BCL::nprocs();

    int me = BCL::rank();
    ll val = (n + num - 1) / num;
    
    ll su = 0;
    for (int i = me * val + 1; i <= std::min(n, (me + 1) * val); i++) {
        su += i;
    }

    ll total = n * (n + 1) / 2;
    ll red = 0;
    red = BCL::reduce(su, 
            [&](ll acc, ll m) -> ll { return acc + m;},
            0);

    if (BCL::rank() == 0) {
        if (total == red) {
            fprintf(stdout, "1 + 2 + ... + %llu = %llu\n", n, red);
        } else {
            fprintf(stderr, "Wrong Logic %lld != %lld", total, red);
            return 1;
        }
    }
    // TODO: your code here

    BCL::finalize();
    return 0;
}
