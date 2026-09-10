// Practice problem: 05_sum_in_range
// TODO(you): implement it yourself with BCL CoreX.
// Hints (which primitives to practice): see the table in ../../../README.md
// Build & run:
//   make build PROB=05_sum_in_range SRC=mine/mine.cpp
//   make run   PROB=05_sum_in_range SRC=mine/mine.cpp NP=4 ARGS="<input>"
#include <bclx/bclx.hpp>
#include <stdio.h>
#include <stdlib.h>

using ll = long long;

ll calc1(ll L, ll R) {
    int me = BCL::rank();
    int np = BCL::nprocs();

    ll n = R - L + 1;
    ll lo = n * me / np;
    ll hi = n * (me + 1) / np;

    ll sum = 0;
    for (int i = lo + L; i < hi + L; i++) {
        sum += i;
    }

    ll total = BCL::allreduce(sum, [](ll a, ll b) -> ll {return a + b;});
    return total;
}

ll calc2(ll L, ll R) {
    int me = BCL::rank();
    int np = BCL::nprocs();

    ll n = R - L + 1;
    BCL::GlobalPtr<ll> chunks = nullptr;

    if (me == 0) {
        chunks = BCL::alloc<ll>(np);
        for (int i = 0; i < np; i++) chunks.local()[i] = 0;
    }
    chunks = BCL::broadcast(chunks, 0);

    // int n = R - L + 1;
    ll lo = n * me / np;
    ll hi = n * (me + 1) / np;

    ll sum = 0;
    for (int i = lo + L; i < hi + L; i++) {
        sum += i;
    }

    bclx::aput_sync(sum, chunks + (BCL::rank()));



    bclx::barrier_sync();



    if (me == 0) {
        ll total = 0;
        for (int i = 0; i < np; i++) total += chunks.local()[i];
        BCL::dealloc<ll>(chunks);
        return total;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <L> <R>\n", argv[0]);
        return 1;
    }

    BCL::init();

    ll L = atoll(argv[1]);
    ll R = atoll(argv[2]);

    ll c1 = calc1(L, R);
    ll c2 = calc2(L, R);

    if (BCL::rank() == 0) {
        if (c1 == c2) {
            fprintf(stdout, "sum from %lld to %lld: %lld\n", L, R, c1);
        } else {
            fprintf(stderr, "Wrong logic %lld != %lld \n", c1, c2);
            return 1;
        }
    }
    // TODO: your code here

    BCL::finalize();
    return 0;
}
