// Practice problem: 04_sum_of_n_numbers
// TODO(you): implement it yourself with BCL CoreX.
// Hints (which primitives to practice): see the table in ../../../README.md
// Build & run:
//   make build PROB=04_sum_of_n_numbers SRC=mine/mine.cpp
//   make run   PROB=04_sum_of_n_numbers SRC=mine/mine.cpp NP=4 ARGS="<input>"
#include <bclx/bclx.hpp>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
using ll = long long;

ll val(ll num, ll n) {
    return (num * 19284173 + 1) % n;
}

ll calc1(int n) {
    int me = BCL::rank();
    int np = BCL::nprocs();

    int lo = n * me / np;
    int hi = n * (me + 1) / np;

    ll sum = 0;
    for (int i = lo; i < hi; i++) sum += val(i, n);

    ll total = BCL::allreduce(sum, [](ll x, ll y) -> ll {return x + y;});
    return total;
}

ll calc2(int n) {
    int me = BCL::rank();
    int np = BCL::nprocs();

    int lo = n * me / np;
    int hi = n * (me + 1) / np;

    int len = hi - lo;
    BCL::GlobalPtr<ll> gptr = BCL::alloc<ll>(len > 0 ? len : 1);
    for (int i = 0; i < len; i++) {
        gptr.local()[i] = val(i + lo, n);
    }

    std::vector<BCL::GlobalPtr<ll>> base(np);
    base[me] = gptr;
    for (int i = 0; i < np; i++) {
        base[i] = BCL::broadcast(base[i], i);
    }

    bclx::barrier_sync();

    ll sum = 0;
    for (int i = 0; i < np; i++) {
        int ilo = n * i / np;
        int iro = n * (i + 1) / np;
        if (iro - ilo == 0) continue;
        std::vector<ll> va(iro - ilo);
        bclx::rget_sync(base[i], va.data(), iro - ilo);
        for (auto x : va) sum += x;
    }

    BCL::dealloc<ll>(gptr);
    return sum;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <N>\n", argv[0]);
        return 1;
    }

    BCL::init();

    ll n = 0;
    if (BCL::rank() == 0) {
        n = atoll(argv[1]);
    }
    BCL::broadcast(n, 0);

    ll c1 = calc1(n);
    ll c2 = calc2(n);

    // TODO: your code here

    if (BCL::rank() == 0) {
        if (c1 == c2) fprintf(stdout, "sum of %lld numbers = %lld\n", n, c1);
        else {
            fprintf(stdout, "Wrong logic %lld != %lld\n", c1, c2);
            return 1;
        }
    }

    BCL::finalize();
    return 0;
}
