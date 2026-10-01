// 65 — Sums of All Subsets | v2: striped top-level prefixes + fao count check
// The include/exclude recursion tree has 2^m prefixes at depth m; choose the
// smallest m with 2^m >= P and stripe the prefixes over the ranks (prefix id
// % P == me). Below depth m everything is local recursion. A fao counter on
// rank 0 verifies that the ranks together produced exactly 2^N sums — the
// same "parallelize the first decision levels" idea as 63/v2.
// Run: make run PROB=65_subset_sums SRC=result/v2_striped_prefixes.cpp NP=4 ARGS="4"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100 + 1;
}

static void sums_rest(const std::vector<uint64_t> &a, size_t i, uint64_t cur,
                      uint64_t &cnt, bool print) {
    if (i == a.size()) {
        ++cnt;
        if (print) printf("%llu ", cur);
        return;
    }
    sums_rest(a, i + 1, cur, cnt, print);
    sums_rest(a, i + 1, cur + a[i], cnt, print);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <N>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t n = 0;
    if (me == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n > 20) n = 20;

    // stripe depth: smallest m with 2^m >= P, capped at n
    uint64_t m = 0;
    while (m < n && (1ull << m) < P) ++m;

    std::vector<uint64_t> a(n);
    for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

    BCL::GlobalPtr<uint64_t> total = nullptr;
    if (me == 0) {
        total = BCL::alloc<uint64_t>(1);
        *total.local() = 0;
    }
    total = BCL::broadcast(total, 0);

    uint64_t count = 0;
    bool print = (n <= 5);   // only print for small sets

    for (uint64_t pref = me; pref < (1ull << m); pref += P) {
        uint64_t base = 0;
        for (uint64_t j = 0; j < m; ++j)
            if ((pref >> j) & 1) base += a[j];   // fixed decisions of the prefix
        if (print) printf("[rank %llu] ", me);
        sums_rest(a, m, base, count, print);    // local recursion below depth m
        if (print) printf("\n");
    }

    bclx::fao_sync(total, count, BCL::plus<uint64_t>{});
    bclx::barrier_sync();

    if (me == 0) {
        uint64_t expect = 1ull << n;
        printf("total subset sums = %llu (expected %llu -> %s)\n",
               total.local()[0], expect,
               total.local()[0] == expect ? "MATCH" : "MISMATCH!");
        BCL::dealloc<uint64_t>(total);
    }

    BCL::finalize();
    return 0;
}
