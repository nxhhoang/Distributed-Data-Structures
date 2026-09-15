// Practice problem: 10_prime_in_range
// TODO(you): implement it yourself with BCL CoreX.
// Hints (which primitives to practice): see the table in ../../../README.md
// Build & run:
//   make build PROB=10_prime_in_range SRC=mine/mine.cpp
//   make run   PROB=10_prime_in_range SRC=mine/mine.cpp NP=4 ARGS="<input>"
#include <bclx/bclx.hpp>
#include <cstdio>
#include <cstdlib>

// using ll = long long;
#define ll uint64_t

bool isPrime(ll x) {
    for (ll i = 2; i * i <= x; i++) {
        if (x % i == 0) return 0;
    }
    return 1;
}

ll count1(ll L, ll R) {
    int me = BCL::rank();
    int np = BCL::nprocs();

    ll n = R - L + 1;
    ll lo = n * me / np;
    ll hi = n * (me + 1) / np;

    ll sum = 0;
    for (ll i = L + lo; i < L + hi; i++) {
        if (isPrime(i)) sum++;
    }

    ll total = BCL::reduce(sum, [](ll x, ll y) -> ll {return x + y;}, 0);
    return total;
}

ll count2(ll L, ll R) {
    int me = BCL::rank();
    int np = BCL::nprocs();

    ll n = R - L + 1;
    ll lo = n * me / np;
    ll hi = n * (me + 1) / np;

    BCL::GlobalPtr<ll> res = nullptr;
    BCL::GlobalPtr<ll> idx = nullptr;

    if (me == 0) {
        res = BCL::alloc<ll>(n);
        idx = BCL::alloc<ll>(1);
        idx.local()[0] = 0;
    }

    res = BCL::broadcast(res, 0);
    idx = BCL::broadcast(idx, 0);

    for (ll i = L + lo; i < L + hi; i++) {
        if (isPrime(i)) {
            // ll pos = bclx::fao_sync(idx, 1ll, BCL::plus<ll>{});
            ll pos = bclx::fao_sync(idx, ll(1), BCL::plus<ll>{});
            bclx::aput_async((ll)1, res + pos);
        }
    }

    bclx::barrier_sync();

    
    ll total = 0;
    if (BCL::rank() == 0) {
        int val = idx.local()[0];
        for (int i = 0; i < val; i++) {
            total += res.local()[i];
        }
    }

    total = BCL::broadcast(total, 0);

    if (BCL::rank() == 0) {
        BCL::dealloc<ll>(res);
        BCL::dealloc<ll>(idx);   
    }

    return total;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <L> <R>\n", argv[0]);
        return 1;
    }

    BCL::init();

    ll L = atoll(argv[1]);
    ll R = atoll(argv[2]);

    // ll L = strtoull(argv[1], nullptr, 10);
    // ll R = strtoull(argv[2], nullptr, 10);

    ll N = 100000000;
    if (L > N || R > N || L <= 0 || R <= 0 || L > R) {
        fprintf(stderr, "Not support these ranges [%llu - %llu]", L, R);
        return 1;
    }

    ll c1 = count1(L, R);
    ll c2 = count2(L, R);
    // TODO: your code here

    if (BCL::rank() == 0) {
        if (c1 == c2) {
            fprintf(stdout, "The number of primes is %llu\n", c1);
        } else {
            fprintf(stderr, "Wrong logic %llu != %llu\n", c1, c2);
            return 1;
        }
    }

    BCL::finalize();
    return 0;
}
