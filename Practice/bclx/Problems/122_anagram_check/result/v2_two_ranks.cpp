// 122 — Anagram Check | v2: two ranks, one string each
// Rank 0 histograms string A, rank 1 histograms string B; both write their
// 26-bucket rows into rank 0's PGAS table with aput, and rank 0 compares the
// rows after a barrier (the "each rank owns one item" split of 28/v2).
// Run: make run PROB=122_anagram_check SRC=result/v2_two_ranks.cpp NP=2 ARGS="listen silent"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    uint64_t len;
};

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <string A> <string B>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    if (P < 2) {
        if (me == 0) fprintf(stderr, "this variant needs NP >= 2\n");
        BCL::finalize();
        return 1;
    }

    str_msg a{}, b{};
    a.len = b.len = 0;
    if (me == 0) {
        strncpy(a.s, argv[1], sizeof(a.s) - 1);
        a.len = strlen(a.s);
        strncpy(b.s, argv[2], sizeof(b.s) - 1);
        b.len = strlen(b.s);
    }
    a = BCL::broadcast(a, 0);
    b = BCL::broadcast(b, 0);

    // rank 0 hosts a 2 x 26 histogram table
    BCL::GlobalPtr<uint64_t> table = nullptr;
    if (me == 0) {
        table = BCL::alloc<uint64_t>(2 * 26);
        for (int i = 0; i < 2 * 26; ++i) table.local()[i] = 0;
    }
    table = BCL::broadcast(table, 0);

    const str_msg &mine = (me == 0) ? a : b;   // rank 0 takes A, rank 1 takes B
    for (uint64_t i = 0; i < mine.len; ++i) {
        unsigned char c = (unsigned char)std::tolower((unsigned char)mine.s[i]);
        if (c >= 'a' && c <= 'z')
            bclx::fao_sync(table + 26 * me + (c - 'a'), 1ull, BCL::plus<uint64_t>{});
    }

    bclx::barrier_sync();

    if (me == 0) {
        bool anagram = true;
        for (int c = 0; c < 26 && anagram; ++c)
            if (table.local()[c] != table.local()[26 + c]) anagram = false;

        printf("\"%s\" and \"%s\" are %sanagrams (histograms computed on 2 ranks)\n",
               a.s, b.s, anagram ? "" : "NOT ");
        BCL::dealloc<uint64_t>(table);
    }

    BCL::finalize();
    return 0;
}
