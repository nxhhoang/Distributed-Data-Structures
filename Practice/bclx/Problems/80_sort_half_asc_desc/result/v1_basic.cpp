// 80 — Sort First Half Ascending, Second Half Descending | v1: two std::sort calls
// Run: make run PROB=80_sort_half_asc_desc SRC=result/v1_basic.cpp NP=4 ARGS="16"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
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
    if (n < 2) n = 2;

    std::vector<uint64_t> a(n);
    for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

    std::sort(a.begin(), a.begin() + n / 2);                       // first half ascending
    std::sort(a.begin() + n / 2, a.end(), std::greater<uint64_t>()); // second half descending

    if (BCL::rank() == 0) {
        printf("sorted (%llu/2 asc + desc): ", n);
        for (uint64_t i = 0; i < n && i < 30; ++i) printf("%llu ", a[i]);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
