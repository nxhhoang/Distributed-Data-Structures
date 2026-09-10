// 06 — Greatest of Two Numbers | v1: broadcast both values
// Run: make run PROB=06_greatest_of_two SRC=result/v1_basic.cpp NP=4 ARGS="17 42"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <a> <b>\n", argv[0]);
        return 1;
    }

    BCL::init();

    long long a = 0, b = 0;
    if (BCL::rank() == 0) {
        a = atoll(argv[1]);
        b = atoll(argv[2]);
    }
    a = BCL::broadcast(a, 0);
    b = BCL::broadcast(b, 0);

    if (BCL::rank() == 0)
        printf("greatest(%lld, %lld) = %lld\n", a, b, a > b ? a : b);

    BCL::finalize();
    return 0;
}
