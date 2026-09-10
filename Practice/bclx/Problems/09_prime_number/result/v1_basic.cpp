// 09 — Prime Number | v1: trial division after broadcast
// Run: make run PROB=09_prime_number SRC=result/v1_basic.cpp NP=4 ARGS="1000000007"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <bclx/bclx.hpp>

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    for (uint64_t d = 2; d * d <= n; ++d)
        if (n % d == 0) return false;
    return true;
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

    bool p = is_prime(n);
    if (BCL::rank() == 0)
        printf("%llu is %sprime\n", n, p ? "" : "NOT ");

    BCL::finalize();
    return 0;
}
