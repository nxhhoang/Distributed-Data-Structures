// 120 — Frequency of Characters | v2: LOCAL histogram per chunk + batched fao
// The letter histogram of 82/v2 applied to a string: each rank counts the
// letters of its CONTIGUOUS chunk into a local table first, then merges the
// non-empty buckets into rank 0's table with one fao each. The total is
// verified against the number of letters in the string.
// Run: make run PROB=120_char_frequency SRC=result/v2_pgas_histogram.cpp NP=4 ARGS="mississippi"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <vector>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    uint64_t len;
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <string>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    str_msg m{};
    m.len = 0;
    if (me == 0) {
        strncpy(m.s, argv[1], sizeof(m.s) - 1);
        m.len = strlen(m.s);
    }
    m = BCL::broadcast(m, 0);
    uint64_t n = m.len;

    BCL::GlobalPtr<uint64_t> table = nullptr;
    if (me == 0) {
        table = BCL::alloc<uint64_t>(26);
        for (int c = 0; c < 26; ++c) table.local()[c] = 0;
    }
    table = BCL::broadcast(table, 0);

    // my contiguous chunk of the string
    uint64_t lo = n * me / P, hi = n * (me + 1) / P;

    std::vector<uint64_t> local(26, 0);
    uint64_t letters = 0;
    for (uint64_t i = lo; i < hi; ++i) {
        unsigned char c = (unsigned char)std::tolower((unsigned char)m.s[i]);
        if (c >= 'a' && c <= 'z') { ++local[c - 'a']; ++letters; }
    }

    // merge: one fao per non-empty bucket
    for (int c = 0; c < 26; ++c)
        if (local[c] > 0)
            bclx::fao_sync(table + c, local[c], BCL::plus<uint64_t>{});

    bclx::barrier_sync();

    if (me == 0) {
        uint64_t total = 0;
        printf("frequencies in \"%s\" (distributed histogram):\n", m.s);
        for (int c = 0; c < 26; ++c) {
            if (table.local()[c] > 0) {
                printf("  %c -> %llu\n", 'a' + c, table.local()[c]);
                total += table.local()[c];
            }
        }
        printf("total letters = %llu\n", total);
        BCL::dealloc<uint64_t>(table);
    }

    BCL::finalize();
    return 0;
}
