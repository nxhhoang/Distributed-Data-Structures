// 164 — Kth Smallest in a Row-Column Sorted Matrix | v2: OWN heap over row heads
// v1 binary-searched the VALUE range; here the classic heap solution: seed a
// min-heap with the first element of every row, then pop k times — after
// popping (r, c), push (r, c+1). The kth pop is the answer. O(k log R).
// (Compare with v1's O(R log C log range) — different complexity axes.)
// Run: make run PROB=164_kth_smallest_matrix SRC=result/v2_own_heap.cpp NP=4 ARGS="4 5 8"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <utility>
#include <bclx/bclx.hpp>

static uint64_t mat_at(uint64_t r, uint64_t c) {
    return 10 * r + 2 * c + 1;
}

// hand-rolled min-heap over (value, row)
struct RowHeap {
    std::vector<std::pair<uint64_t, uint64_t>> a;
    void push(uint64_t v, uint64_t r) {
        a.push_back({v, r});
        size_t i = a.size() - 1;
        while (i > 0) {
            size_t p = (i - 1) / 2;
            if (a[p].first <= a[i].first) break;
            std::swap(a[p], a[i]);
            i = p;
        }
    }
    std::pair<uint64_t, uint64_t> pop() {
        std::pair<uint64_t, uint64_t> res = a[0];
        a[0] = a.back();
        a.pop_back();
        size_t i = 0;
        while (true) {
            size_t l = 2 * i + 1, r2 = l + 1, m = i;
            if (l < a.size() && a[l].first < a[m].first) m = l;
            if (r2 < a.size() && a[r2].first < a[m].first) m = r2;
            if (m == i) break;
            std::swap(a[i], a[m]);
            i = m;
        }
        return res;
    }
};

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "usage: %s <rows> <cols> <k>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t R = 0, C = 0, k = 0;
    if (BCL::rank() == 0) {
        R = strtoull(argv[1], nullptr, 10);
        C = strtoull(argv[2], nullptr, 10);
        k = strtoull(argv[3], nullptr, 10);
    }
    R = BCL::broadcast(R, 0);
    C = BCL::broadcast(C, 0);
    k = BCL::broadcast(k, 0);
    if (R == 0) R = 1;
    if (C == 0) C = 1;
    if (k == 0 || k > R * C) k = 1;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> col(R, 0);
        RowHeap h;
        for (uint64_t r = 0; r < R; ++r)
            h.push(mat_at(r, 0), r);

        uint64_t answer = 0;
        for (uint64_t step = 0; step < k; ++step) {
            auto top = h.pop();
            answer = top.first;
            uint64_t r = top.second;
            ++col[r];
            if (col[r] < C)
                h.push(mat_at(r, col[r]), r);
        }

        printf("%llu-th smallest element = %llu (own heap over row heads)\n", k, answer);
    }

    BCL::finalize();
    return 0;
}
