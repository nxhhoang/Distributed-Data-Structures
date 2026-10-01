// 139 — Count Inversions | v1: merge sort counting
// Every cross-frontier swap in the merge step is one inversion pair; sorting
// and counting happen in the same pass. Sequential by nature (group C).
// Run: make run PROB=139_count_inversions SRC=result/v1_basic.cpp NP=4 ARGS="12"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
}

// sorts v[l..r) and returns how many inversions were inside
static uint64_t merge_count(std::vector<uint64_t> &v, uint64_t l, uint64_t r) {
    if (r - l < 2) return 0;
    uint64_t mid = l + (r - l) / 2;
    uint64_t inv = merge_count(v, l, mid) + merge_count(v, mid, r);

    std::vector<uint64_t> tmp;
    tmp.reserve(r - l);
    uint64_t i = l, j = mid;
    while (i < mid && j < r) {
        if (v[i] <= v[j]) tmp.push_back(v[i++]);
        else {
            inv += mid - i;   // v[i..mid) all come AFTER v[j]: inversions
            tmp.push_back(v[j++]);
        }
    }
    while (i < mid) tmp.push_back(v[i++]);
    while (j < r) tmp.push_back(v[j++]);

    for (uint64_t k = 0; k < tmp.size(); ++k) v[l + k] = tmp[k];
    return inv;
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
    if (n == 0) n = 1;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> v(n);
        for (uint64_t i = 0; i < n; ++i) v[i] = value_at(i);

        printf("inversions = %llu (merge sort count)\n", merge_count(v, 0, n));
    }

    BCL::finalize();
    return 0;
}
