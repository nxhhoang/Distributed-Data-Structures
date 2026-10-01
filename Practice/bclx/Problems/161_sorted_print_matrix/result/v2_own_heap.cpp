// 161 — Print Sorted Order From a Row-Column Sorted Matrix | v2: OWN k-way merge heap
// v1 scanned all row heads linearly (O(R) per element); here a hand-rolled
// min-heap of (value, row) keeps the merge at O(log R) per element. When a
// head is emitted, the next element of its row enters the heap.
// Run: make run PROB=161_sorted_print_matrix SRC=result/v2_own_heap.cpp NP=4 ARGS="4 5"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <utility>
#include <bclx/bclx.hpp>

static uint64_t mat_at(uint64_t r, uint64_t c) {
    return 10 * r + 2 * c + 1;
}

// hand-rolled min-heap over (value, row) pairs
struct RowHeap {
    std::vector<std::pair<uint64_t, uint64_t>> a;   // (value, row)
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
    if (argc < 3) {
        fprintf(stderr, "usage: %s <rows> <cols>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t R = 0, C = 0;
    if (BCL::rank() == 0) {
        R = strtoull(argv[1], nullptr, 10);
        C = strtoull(argv[2], nullptr, 10);
    }
    R = BCL::broadcast(R, 0);
    C = BCL::broadcast(C, 0);
    if (R == 0) R = 1;
    if (C == 0) C = 1;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> head(R, 0);   // current column per row
        RowHeap h;
        for (uint64_t r = 0; r < R; ++r)
            h.push(mat_at(r, 0), r);

        printf("sorted (own k-way heap): ");
        for (uint64_t step = 0; step < R * C; ++step) {
            auto top = h.pop();
            printf("%llu ", top.first);
            uint64_t r = top.second;
            ++head[r];
            if (head[r] < C)
                h.push(mat_at(r, head[r]), r);
        }
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
