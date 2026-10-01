// 122 — Check if Two Strings Are Anagrams | v3: BCL::HashMap +/- counting
// One shared map: rank 0 adds +1 per letter of A, rank 1 adds -1 per letter
// of B (concurrent modifies on the same letter are atomic read-modify-writes).
// Anagram  <=>  lengths equal and every letter count is 0.
//   rank 0 pre-seeds all 26 letters with 0 first (avoids modify-on-fresh).
// Run: make run PROB=122_anagram_check SRC=result/v3_hashmap.cpp NP=2 ARGS="listen silent"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <bclx/bclx.hpp>
#include <bcl/containers/HashMap.hpp>

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

    BCL::HashMap<uint64_t, uint64_t> map(64);

    // rank 0 pre-seeds all 26 letters with 0
    if (me == 0)
        for (int c = 0; c < 26; ++c)
            map.insert_or_assign(c, 0);
    bclx::barrier_sync();

    // rank 0: +1 for each letter of A; rank 1: -1 for each letter of B
    if (me == 0 || P == 1) {
        for (uint64_t i = 0; i < a.len; ++i) {
            unsigned char c = (unsigned char)std::tolower((unsigned char)a.s[i]);
            if (c >= 'a' && c <= 'z')
                map.modify(c - 'a', [](uint64_t x) { return x + 1; });
        }
    }
    if (me == 1 || P == 1) {
        for (uint64_t i = 0; i < b.len; ++i) {
            unsigned char c = (unsigned char)std::tolower((unsigned char)b.s[i]);
            if (c >= 'a' && c <= 'z')
                map.modify(c - 'a', [](uint64_t x) { return x - 1; });
        }
    }

    bclx::barrier_sync();

    // every rank checks its local segment for non-zero counts
    uint64_t bad = 0;
    for (auto it = map.local_begin(); it != map.local_end(); ++it) {
        std::pair<const uint64_t, uint64_t> kv = *it;   // *it is a reference; convert explicitly
        if (kv.second != 0) ++bad;
    }
    uint64_t total_bad = bclx::allreduce(bad, BCL::sum<uint64_t>{});

    if (me == 0)
        printf("\"%s\" and \"%s\" are %sanagrams (HashMap +1/-1 counting)\n",
               a.s, b.s, (a.len == b.len && total_bad == 0) ? "" : "NOT ");

    BCL::finalize();
    return 0;
}
