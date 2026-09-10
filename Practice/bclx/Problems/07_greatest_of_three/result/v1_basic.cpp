// 07 — Greatest of the Three Numbers | v1: three broadcasts
// Run: make run PROB=07_greatest_of_three SRC=result/v1_basic.cpp NP=4 ARGS="17 42 23"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "usage: %s <a> <b> <c>\n", argv[0]);
        return 1;
    }

    BCL::init();

    long long a = 0, b = 0, c = 0;
    if (BCL::rank() == 0) {
        a = atoll(argv[1]);
        b = atoll(argv[2]);
        c = atoll(argv[3]);
    }
    a = BCL::broadcast(a, 0);
    b = BCL::broadcast(b, 0);
    c = BCL::broadcast(c, 0);

    long long m = a;
    if (b > m) m = b;
    if (c > m) m = c;

    if (BCL::rank() == 0)
        printf("greatest(%lld, %lld, %lld) = %lld\n", a, b, c, m);

    BCL::finalize();
    return 0;
}
