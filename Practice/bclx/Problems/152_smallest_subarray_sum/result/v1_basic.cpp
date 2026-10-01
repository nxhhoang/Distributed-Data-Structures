// 152 — Smallest Subarray With Sum Greater Than a Given Value | v1: sliding window
// Expand the right edge; once the window sum exceeds x, shrink from the left
// while it still does — the classic two-pointer window.
// Run: make run PROB=152_smallest_subarray_sum SRC=result/v1_basic.cpp NP=4 ARGS="12 60"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100 + 1;   // all positive
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <n> <x>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0, x = 0;
    if (BCL::rank() == 0) {
        n = strtoull(argv[1], nullptr, 10);
        x = strtoull(argv[2], nullptr, 10);
    }
    n = BCL::broadcast(n, 0);
    x = BCL::broadcast(x, 0);
    if (n == 0) n = 1;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

        uint64_t best_len = ~0ull, best_s = 0;
        uint64_t sum = 0, start = 0;
        for (uint64_t end = 0; end < n; ++end) {
            sum += a[end];
            while (sum > x) {
                if (end - start + 1 < best_len) { best_len = end - start + 1; best_s = start; }
                sum -= a[start];
                ++start;
            }
        }

        if (best_len == ~0ull)
            printf("no subarray has a sum greater than %llu\n", x);
        else
            printf("smallest subarray with sum > %llu: length %llu over [%llu..%llu]\n",
                   x, best_len, best_s, best_s + best_len - 1);
    }

    BCL::finalize();
    return 0;
}
