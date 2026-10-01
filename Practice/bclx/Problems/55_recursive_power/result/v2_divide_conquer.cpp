// 55 — Power of a Number (recursion) | v2: divide-and-conquer halving
// power(b, e) = half^2 (x b when e is odd) with half = power(b, e/2) —
// O(log e) recursion depth instead of O(e). Same shape many DMM algorithms
// use to split work recursively.
// Run: make run PROB=55_recursive_power SRC=result/v2_divide_conquer.cpp NP=4 ARGS="3 19"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static uint64_t power_dc(uint64_t b, uint64_t e) {
    if (e == 0) return 1;
    uint64_t half = power_dc(b, e / 2);
    return (e & 1) ? half * half * b : half * half;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <base> <exp>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t b = 0, e = 0;
    if (BCL::rank() == 0) {
        b = strtoull(argv[1], nullptr, 10);
        e = strtoull(argv[2], nullptr, 10);
    }
    b = BCL::broadcast(b, 0);
    e = BCL::broadcast(e, 0);

    if (BCL::rank() == 0)
        printf("%llu^%llu = %llu (divide and conquer)\n", b, e, power_dc(b, e));

    BCL::finalize();
    return 0;
}
