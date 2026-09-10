// 01 — Positive or Negative number | v1: broadcast + SPMD check
// Rank 0 parses the number and broadcasts it; every rank classifies it
// (SPMD style), rank 0 prints. Primitives: BCL::init/finalize, BCL::broadcast.
// Run: make run PROB=01_positive_or_negative SRC=result/v1_basic.cpp NP=4 ARGS="-42"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        return 1;
    }

    BCL::init();

    long long n = 0;
    if (BCL::rank() == 0) n = atoll(argv[1]);
    n = BCL::broadcast(n, 0);   // collective: every rank receives rank 0's value

    const char *ans = (n > 0) ? "Positive" : (n < 0) ? "Negative" : "Zero";
    if (BCL::rank() == 0)
        printf("%lld is %s\n", n, ans);

    BCL::finalize();
    return 0;
}
