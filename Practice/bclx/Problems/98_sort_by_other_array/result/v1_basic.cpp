// 98 — Sort an Array According to the Order Defined by Another Array | v1
// Elements of A that appear in B are ordered by B's position; the leftovers
// are appended in ascending order.
// Run: make run PROB=98_sort_by_other_array SRC=result/v1_basic.cpp NP=4 ARGS="12 5"

#include <cstdio>
#include <cstdlib>
#include <map>
#include <vector>
#include <algorithm>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
}

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

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n), b(m);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);
        for (uint64_t i = 0; i < m; ++i) b[i] = value_at(i + 7777);

        std::map<uint64_t, uint64_t> pos;   // value -> position in B
        for (uint64_t i = 0; i < m; ++i)
            if (!pos.count(b[i])) pos[b[i]] = i;

        std::stable_sort(a.begin(), a.end(), [&](uint64_t x, uint64_t y) {
            bool px = pos.count(x), py = pos.count(y);
            if (px && py) return pos[x] < pos[y];   // both ordered: B's order
            if (px != py) return px;                  // ordered ones come first
            return x < y;                              // leftovers ascending
        });

        printf("A sorted by B's order: ");
        for (uint64_t v : a) printf("%llu ", v);
        printf("\nB was: ");
        for (uint64_t v : b) printf("%llu ", v);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
