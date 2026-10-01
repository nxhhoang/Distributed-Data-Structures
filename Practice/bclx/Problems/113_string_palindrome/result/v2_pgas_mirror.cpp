// 113 — Palindrome String | v2: distributed mirror compare
// The string is laid out as a distributed array of chars (one u64 slot per
// char, contiguous chunks in each rank's PGAS heap). Every rank compares its
// own chunk against the MIRRORED region read remotely with aget (element i
// vs element n-1-i); the mismatch counts are summed with allreduce. This is
// the read-only twin of 79/v2's mirrored swap.
// Run: make run PROB=113_string_palindrome SRC=result/v2_pgas_mirror.cpp NP=4 ARGS="racecar"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    uint64_t len;
};

// owner of global index m under contiguous chunking [n*r/P, n*(r+1)/P)
static uint64_t owner_of(uint64_t m, uint64_t n, uint64_t P) {
    for (uint64_t r = 0; r < P; ++r)
        if (m >= n * r / P && m < n * (r + 1) / P) return r;
    return P - 1;
}

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
    if (n == 0) n = 1;

    // my chunk of the distributed char array
    uint64_t lo = n * me / P, hi = n * (me + 1) / P;
    uint64_t len = hi - lo;
    BCL::GlobalPtr<uint64_t> chunk = BCL::alloc<uint64_t>(len > 0 ? len : 1);
    for (uint64_t j = 0; j < len; ++j)
        chunk.local()[j] = (uint64_t)(unsigned char)m.s[lo + j];

    std::vector<BCL::GlobalPtr<uint64_t>> bases(P);
    bases[me] = chunk;
    for (uint64_t r = 0; r < P; ++r)
        bases[r] = BCL::broadcast(bases[r], r);

    // compare my slots against the mirrored ones (remote aget)
    uint64_t bad = 0;
    for (uint64_t i = lo; i < hi; ++i) {
        uint64_t mir = n - 1 - i;
        uint64_t r = owner_of(mir, n, P);
        uint64_t v = bclx::aget_sync(bases[r] + (mir - n * r / P));
        if (v != chunk.local()[i - lo]) ++bad;
    }

    uint64_t total_bad = bclx::allreduce(bad, BCL::sum<uint64_t>{});

    if (me == 0)
        printf("\"%s\" is %sa palindrome (distributed mirror compare)\n",
               m.s, total_bad == 0 ? "" : "NOT ");

    BCL::dealloc<uint64_t>(chunk);
    BCL::finalize();
    return 0;
}
