// 131 — Union and Intersection of Two Sorted Arrays | v1: merge walk
// A = multiples of 3, B = multiples of 4 — sorted, distinct, overlapping.
// Run: make run PROB=131_union_intersection SRC=result/v1_basic.cpp NP=4 ARGS="10 12"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_a(uint64_t i) { return 3 * i; }
static uint64_t value_b(uint64_t i) { return 4 * i; }

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n A> <m B>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0, m = 0;
    if (BCL::rank() == 0) {
        n = strtoull(argv[1], nullptr, 10);
        m = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    m = BCL::broadcast(m, 0);
    if (n == 0) n = 1;
    if (m == 0) m = 1;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n), b(m);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_a(i);
        for (uint64_t i = 0; i < m; ++i) b[i] = value_b(i);

        // merge walk
        std::vector<uint64_t> un, inter;
        uint64_t i = 0, j = 0;
        while (i < n && j < m) {
            if (a[i] < b[j]) un.push_back(a[i++]);
            else if (b[j] < a[i]) un.push_back(b[j++]);
            else { un.push_back(a[i]); inter.push_back(a[i]); ++i; ++j; }
        }
        while (i < n) un.push_back(a[i++]);
        while (j < m) un.push_back(b[j++]);

        printf("union (%zu): ", un.size());
        for (uint64_t v : un) printf("%llu ", v);
        printf("\nintersection (%zu): ", inter.size());
        for (uint64_t v : inter) printf("%llu ", v);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
