// 145 — Alternating Positive and Negative Items With O(1) Extra Space | v1
// The right-rotation method: at each slot, when the expected sign is wrong,
// find the NEXT slot with the wrong sign for ITS side and right-rotate the
// segment between them so both fall into place. O(n^2) time, O(1) space —
// the price of not using an auxiliary array.
// Run: make run PROB=145_alternating_pos_neg SRC=result/v1_basic.cpp NP=4 ARGS="12"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static int64_t value_signed(uint64_t i) {
    return (int64_t)((i * 2654435761ull + 17) % 200) - 100;
}

static void right_rotate(std::vector<int64_t> &a, uint64_t from, uint64_t to) {
    int64_t tmp = a[to];
    for (uint64_t i = to; i > from; --i) a[i] = a[i - 1];
    a[from] = tmp;
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
        std::vector<int64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_signed(i);

        int64_t want = 1;   // +1: expect non-negative, -1: expect negative
        for (uint64_t i = 0; i < n; ++i) {
            bool ok = (want > 0) ? (a[i] >= 0) : (a[i] < 0);
            if (!ok) {
                // find the next element with the sign expected HERE
                uint64_t j = i + 1;
                while (j < n) {
                    bool okj = (want > 0) ? (a[j] >= 0) : (a[j] < 0);
                    if (okj) break;
                    ++j;
                }
                if (j == n) break;   // ran out of that sign
                right_rotate(a, i, j);
            }
            want = -want;
        }

        printf("alternating: ");
        for (uint64_t i = 0; i < n && i < 24; ++i) printf("%lld ", a[i]);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
