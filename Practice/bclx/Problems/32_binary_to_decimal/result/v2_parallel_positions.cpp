// 32 — Binary to Decimal conversion | v2: striped bit positions + allreduce
// Bit at string position j carries weight 2^(len-1-j); each rank accumulates
// only the weights congruent to itself mod P, then the partials are summed.
// A tiny data-parallel reduction over positional notation.
// Run: make run PROB=32_binary_to_decimal SRC=result/v2_parallel_positions.cpp NP=4 ARGS="101101"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    uint64_t len;
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <binary string>\n", argv[0]);
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

    if (m.len > 63) m.len = 63;   // uint64 overflow guard

    // I own weight exponents e with e % P == me
    uint64_t partial = 0;
    for (uint64_t j = 0; j < m.len; ++j) {
        uint64_t e = m.len - 1 - j;
        if (e % P == me && m.s[j] == '1')
            partial += 1ull << e;
    }

    uint64_t total = bclx::allreduce(partial, BCL::sum<uint64_t>{});

    if (me == 0)
        printf("binary %s = %llu decimal (positions striped over %llu ranks)\n",
               m.s, total, P);

    BCL::finalize();
    return 0;
}
