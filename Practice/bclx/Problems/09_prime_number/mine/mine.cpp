// Practice problem: 09_prime_number
// TODO(you): implement it yourself with BCL CoreX.
// Hints (which primitives to practice): see the table in ../../../README.md
// Build & run:
//   make build PROB=09_prime_number SRC=mine/mine.cpp
//   make run   PROB=09_prime_number SRC=mine/mine.cpp NP=4 ARGS="<input>"
#include <bclx/bclx.hpp>
#include <stdio.h>
#include <stdlib.h>
using ll = long long;

ll checkPrime(ll n) {
    int me = BCL::rank();
    int np = BCL::nprocs();

    ll sqr = std::sqrt(n + 2);
    ll lo = std::max(sqr * me / np, 2ll);
    ll hi = std::min(sqr * (me + 1) / np, n);

    int sum = 0;
    for (int i = lo; i < hi; i++) {
        if (n % i == 0) sum++;
    }
    
    ll total = BCL::reduce(sum, [](ll a, ll b) -> ll {return a+b;}, 0);
    
    if (total == 0) return 1;
    return 0;
}   

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage %s <n>", argv[0]);
        return 1;
    }
    
    BCL::init();

    ll n = atoll(argv[1]);
    bool c1 = checkPrime(n);
    
    if (BCL::rank() == 0) {
        if (c1) {
            fprintf(stdout, "%lld is prime\n", n);
        } 
        else {
            fprintf(stdout, "%lld is NOT prime\n", n);
        }
    }

    // TODO: your code here

    BCL::finalize();
    return 0;
}
