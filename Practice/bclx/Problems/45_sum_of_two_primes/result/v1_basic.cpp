// 45 — Can a Number be Expressed as a Sum of Two Prime Numbers | v1: rank 0 scan
// For each prime candidate p in [2..n/2], check whether both p and n-p are
// prime (a Goldbach-style check).
// Run: make run PROB=45_sum_of_two_primes SRC=result/v1_basic.cpp NP=4 ARGS="74"

#include <cstdio>
#include <cstdlib>
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

    if (BCL::rank() == 0) {
        bool found = false;
        for (uint64_t p = 2; p <= n / 2; ++p) {
            if (is_prime(p) && is_prime(n - p)) {
                printf("%llu = %llu + %llu (both prime)\n", n, p, n - p);
                found = true;
                break;
            }
        }
        if (!found)
            printf("%llu cannot be expressed as a sum of two primes\n", n);
    }

    BCL::finalize();
    return 0;
}
