// 02 — Even or Odd number | v1: broadcast + modulo
// Same pattern as 01/v1. Run: ... ARGS="7"
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
    n = BCL::broadcast(n, 0);

    const char *ans = (n % 2 == 0) ? "Even" : "Odd";
    if (BCL::rank() == 0)
        printf("%lld is %s\n", n, ans);

    BCL::finalize();
    return 0;
}
