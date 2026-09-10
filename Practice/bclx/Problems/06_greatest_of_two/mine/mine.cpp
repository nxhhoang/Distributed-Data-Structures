// Practice problem: 06_greatest_of_two
// TODO(you): implement it yourself with BCL CoreX.
// Hints (which primitives to practice): see the table in ../../../README.md
// Build & run:
//   make build PROB=06_greatest_of_two SRC=mine/mine.cpp
//   make run   PROB=06_greatest_of_two SRC=mine/mine.cpp NP=4 ARGS="<input>"
#include <bclx/bclx.hpp>
#include <stdio.h>
#include <stdlib.h>
using ll = long long;

ll calc1(ll a, ll b) {
    int me = BCL::rank();
    int np = BCL::nprocs();

    ll A = 0;
    ll B = 0;

    if (me == 0) {
        A = a;
        B = b;
    }

    A = BCL::broadcast(a, 0);
    B = BCL::broadcast(b, 0);

    return (A > B) ? A : B; 
}

ll calc2(ll a, ll b) {
    int me = BCL::rank();
    int np = BCL::nprocs();

    BCL::GlobalPtr<ll> num;
    if (me == 0) {
        num = BCL::alloc<ll>(2);
        num.local()[0] = a;
        num.local()[1] = b;
    }
    num = BCL::broadcast(num, 0);

    a = bclx::aget_sync(num + 0);
    b = bclx::aget_sync(num + 1);

    bool val = (a > b);
    ll total = BCL::reduce(val, [](ll c, ll d) -> ll {return c + d;}, 0);

    if (me == 0) {
        BCL::dealloc<ll>(num);
    }
    if (total == (ll)np) return a;
    return b; 
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage %s <a> <b>\n", argv[0]);
        return 1;
    }
    
    BCL::init();
    ll a = atoll(argv[1]);
    ll b = atoll(argv[2]);

    ll c1 = calc1(a, b);
    ll c2 = calc2(a, b);

    if (BCL::rank() == 0) {
        if (c1 == c2) {
            fprintf(stdout, "greatest(%lld, %lld) = %lld\n", a, b, c1);
        } else {
            fprintf(stderr, "Wrong logic %lld != %lld\n", c1, c2);
            return 1;
        }
    }

    // TODO: your code here

    BCL::finalize();
    return 0;
}
