// 146 — Find All Elements That Appear More Than n/k Times | v1: sort + runs
// Run: make run PROB=146_more_than_n_by_k SRC=result/v1_basic.cpp NP=4 ARGS="30 4"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 50;   // few buckets -> frequent repeats
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n> <k>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0, k = 0;
    if (BCL::rank() == 0) {
        n = strtoull(argv[1], nullptr, 10);
        k = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    k = BCL::broadcast(k, 0);
    if (n == 0) n = 1;
    if (k == 0) k = 1;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);
        std::sort(a.begin(), a.end());

        uint64_t threshold = n / k;   // strictly more than n/k
        printf("elements appearing more than %llu times (n/k = %llu/%llu): ",
               threshold, n, k);

        uint64_t run = 0;
        bool any = false;
        for (uint64_t i = 0; i < n; ++i) {
            ++run;
            if (i + 1 == n || a[i + 1] != a[i]) {
                if (run > threshold) {
                    printf("%llu(%llux) ", a[i], run);
                    any = true;
                }
                run = 0;
            }
        }
        if (!any) printf("(none)");
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
