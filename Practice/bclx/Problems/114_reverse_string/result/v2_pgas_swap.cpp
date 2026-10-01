// 114 — Print the String in Reverse Order | v2: PGAS mirrored swap of a char array
// The string lives as a distributed array of u64 slots. Every rank agets its
// mirrored region into a local buffer, a barrier makes sure everyone has read
// the OLD values, then each rank writes the buffer into its own slots. Rank 0
// agets the final array in order and prints it; a mismatch count verifies
// element i ended up as the original character n-1-i.
// Run: make run PROB=114_reverse_string SRC=result/v2_pgas_swap.cpp NP=4 ARGS="hello world"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    uint64_t len;
};

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

    uint64_t lo = n * me / P, hi = n * (me + 1) / P;
    uint64_t len = hi - lo;
    BCL::GlobalPtr<uint64_t> chunk = BCL::alloc<uint64_t>(len > 0 ? len : 1);
    for (uint64_t j = 0; j < len; ++j)
        chunk.local()[j] = (uint64_t)(unsigned char)m.s[lo + j];

    std::vector<BCL::GlobalPtr<uint64_t>> bases(P);
    bases[me] = chunk;
    for (uint64_t r = 0; r < P; ++r)
        bases[r] = BCL::broadcast(bases[r], r);

    // phase 1: read the mirrored region
    std::vector<uint64_t> buf(len);
    for (uint64_t i = lo; i < hi; ++i) {
        uint64_t mir = n - 1 - i;
        uint64_t r = owner_of(mir, n, P);
        buf[i - lo] = bclx::aget_sync(bases[r] + (mir - n * r / P));
    }

    bclx::barrier_sync();   // everyone has read before anyone writes

    // phase 2: write into my own slots
    for (uint64_t j = 0; j < len; ++j)
        chunk.local()[j] = buf[j];

    bclx::barrier_sync();

    // rank 0 gathers the final array and prints; verify the swap too
    if (me == 0) {
        uint64_t bad = 0;
        printf("\"%s\" reversed (distributed swap) = \"", m.s);
        for (uint64_t i = 0; i < n; ++i) {
            uint64_t r = owner_of(i, n, P);
            uint64_t v = bclx::aget_sync(bases[r] + (i - n * r / P));
            if (v != (uint64_t)(unsigned char)m.s[n - 1 - i]) ++bad;
            putchar((char)v);
        }
        printf("\" (%s)\n", bad == 0 ? "verified" : "MISMATCH!");
    }

    BCL::dealloc<uint64_t>(chunk);
    BCL::finalize();
    return 0;
}
