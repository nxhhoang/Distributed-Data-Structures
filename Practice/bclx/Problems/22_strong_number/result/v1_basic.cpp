// 22 — Strong Number | v1: broadcast + digit factorial sum
// A strong number equals the sum of the factorials of its digits
// (e.g. 145 = 1! + 4! + 5!). Note 0! = 1.
// Run: make run PROB=22_strong_number SRC=result/v1_basic.cpp NP=4 ARGS="145"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);

    uint64_t fact10[10] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};
    uint64_t sum = 0;
    for (uint64_t x = n; x > 0; x /= 10) sum += fact10[x % 10];

    bool strong = (sum == n);
    if (BCL::rank() == 0)
        printf("%llu is %sa strong number (digit factorial sum = %llu)\n",
               n, strong ? "" : "NOT ", sum);

    BCL::finalize();
    return 0;
}
