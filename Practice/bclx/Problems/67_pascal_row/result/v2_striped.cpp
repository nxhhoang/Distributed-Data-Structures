// 67 — Nth Row of Pascal's Triangle | v2: striped k + aput row collection
// The entries of the row are INDEPENDENT of each other, so rank me computes
// the entries k with k % P == me (each with a local multiplicative loop over
// min(k, n-k) factors) and writes them straight into slot k of rank 0's row
// array with aput_sync.
// Run: make run PROB=67_pascal_row SRC=result/v2_striped.cpp NP=4 ARGS="10"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

// C(n, k) by the multiplicative recurrence: C(n, k) = C(n, k-1) * (n-k+1) / k
static uint64_t binom(uint64_t n, uint64_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    uint64_t r = 1;
    for (uint64_t i = 0; i < k; ++i) {
        r = r * (n - i) / (i + 1);
    }
    return r;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t n = 0;
    if (me == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n > 61) n = 61;   // C(61,30) still fits in uint64

    BCL::GlobalPtr<uint64_t> row = nullptr;
    if (me == 0) {
        row = BCL::alloc<uint64_t>(n + 1);
        for (uint64_t k = 0; k <= n; ++k) row.local()[k] = 0;
    }
    row = BCL::broadcast(row, 0);

    for (uint64_t k = me; k <= n; k += P)
        bclx::aput_sync(binom(n, k), row + k);

    bclx::barrier_sync();

    if (me == 0) {
        printf("row %llu of Pascal's triangle (striped over %llu ranks): ", n, P);
        for (uint64_t k = 0; k <= n; ++k)
            printf("%llu ", row.local()[k]);
        printf("\n");
        BCL::dealloc<uint64_t>(row);
    }

    BCL::finalize();
    return 0;
}
