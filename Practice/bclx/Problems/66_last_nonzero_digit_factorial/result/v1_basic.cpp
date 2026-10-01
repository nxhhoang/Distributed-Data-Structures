// 66 — Last Non-Zero Digit in Factorial | v1: the classic D(n) recursion
// dig[k] = last non-zero digit of k!  ->  {1,1,2,6,4,2,2,4,2,8}.
// For n >= 10: D(n) = (6 or 4) * D(n/5) * dig[n%10] mod 10, where the
// multiplier is 6 when the tens digit of n is even, 4 when odd.
// Run: make run PROB=66_last_nonzero_digit_factorial SRC=result/v1_basic.cpp NP=4 ARGS="25"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static const uint64_t DIG[10] = {1, 1, 2, 6, 4, 2, 2, 4, 2, 8};

static uint64_t lastd(uint64_t n) {
    if (n < 10) return DIG[n];
    uint64_t mult = (((n / 10) % 10) % 2 == 0) ? 6 : 4;
    return (mult * lastd(n / 5) * DIG[n % 10]) % 10;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);

    if (BCL::rank() == 0)
        printf("last non-zero digit of %llu! = %llu (recursion)\n", n, lastd(n));

    BCL::finalize();
    return 0;
}
