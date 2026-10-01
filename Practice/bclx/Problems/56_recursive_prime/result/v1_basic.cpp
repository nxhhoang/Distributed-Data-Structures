// 56 — Prime Number (recursion) | v1: recursive divisor check
// is_prime(n, d): d is the divisor currently being tried.
// Run: make run PROB=56_recursive_prime SRC=result/v1_basic.cpp NP=4 ARGS="1000000007"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

static bool is_prime_rec(uint64_t n, uint64_t d) {
    if (d * d > n) return true;    // no divisor up to sqrt(n)
    if (n % d == 0) return false;
    return is_prime_rec(n, d + 1);
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

    bool p = (n >= 2) && is_prime_rec(n, 2);
    if (BCL::rank() == 0)
        printf("%llu is %sprime (recursive check)\n", n, p ? "" : "NOT ");

    BCL::finalize();
    return 0;
}
