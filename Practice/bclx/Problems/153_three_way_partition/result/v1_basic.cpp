// 153 — Three-Way Partitioning of an Array Around a Given Value/Range | v1
// Dutch-National-Flag variant over the range [low, high]:
//   [< low] [low..high] [> high]  — one pass, three regions.
// Run: make run PROB=153_three_way_partition SRC=result/v1_basic.cpp NP=4 ARGS="16 30 70"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "usage: %s <n> <low> <high>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0, lo = 0, hi = 0;
    if (BCL::rank() == 0) {
        n = strtoull(argv[1], nullptr, 10);
        lo = strtoull(argv[2], nullptr, 10);
        hi = strtoull(argv[3], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    lo = BCL::broadcast(lo, 0);
    hi = BCL::broadcast(hi, 0);
    if (n == 0) n = 1;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

        uint64_t start = 0, mid = 0, end = n - 1;
        while (mid <= end) {
            if (a[mid] < lo) {
                uint64_t t = a[start]; a[start] = a[mid]; a[mid] = t;
                ++start; ++mid;
            } else if (a[mid] > hi) {
                uint64_t t = a[mid]; a[mid] = a[end]; a[end] = t;
                if (end == 0) break;
                --end;
            } else {
                ++mid;
            }
        }

        printf("partitioned around [%llu, %llu]: ", lo, hi);
        for (uint64_t i = 0; i < n && i < 40; ++i) printf("%llu ", a[i]);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
