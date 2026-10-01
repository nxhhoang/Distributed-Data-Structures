// 114 — Print the String in Reverse Order | v2: distributed Treiber stack
// The whole point of a stack: reverse order comes free. The string is
// reversed DISTRIBUTED: rank r pushes its chunk in RANK ORDER phases
// (0, 1, ..., P-1), then pops in REVERSE rank order phases (P-1 ... 0) —
// LIFO + the two phase orders make rank r pop EXACTLY the reverse of its
// own chunk (deterministic, so each rank can verify locally), and the print
// order across phases is the full reversed string.
// Run: make run PROB=114_reverse_string SRC=result/v2_distributed_stack.cpp NP=4 ARGS="hello world"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <bclx/bclx.hpp>

#include <memory/inc/sds.h>

namespace dds {
    const uint64_t MASTER_UNIT = 0;
    const uint64_t TOTAL_OPS = 32768;
    std::string stack_name;
    std::string mem_manager;
    uint64_t bk_init = 2;
    uint64_t bk_max = 1ull << 20;
}

#include <memory/inc/memory_nmr.h>
#include <stack/inc/stack_treiber.h>

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
    if (n == 0) n = 1;

    dds::ts::stack<char> st;   // collective

    uint64_t lo = n * me / P, hi = n * (me + 1) / P;

    // phase 1: pushes in rank order — bottom of the stack is rank 0's chunk
    for (uint64_t r = 0; r < P; ++r) {
        if (r == me)
            for (uint64_t i = lo; i < hi; ++i)
                st.push(m.s[i]);
        bclx::barrier_sync();
    }

    // phase 2: pops in REVERSE rank order — rank r receives its own chunk reversed
    char out[80];
    uint64_t k = 0, bad = 0;
    for (int64_t r = (int64_t)P - 1; r >= 0; --r) {
        if ((uint64_t)r == me) {
            for (uint64_t i = lo; i < hi; ++i) {
                char c;
                st.pop(c);
                out[k++] = c;
            }
            // print my part of the reversed string (pop order)
            for (uint64_t i = 0; i < k; ++i) putchar(out[i]);
            k = 0;
        }
        bclx::barrier_sync();
    }

    // verification: what I popped must be MY chunk reversed
    for (uint64_t i = lo; i < hi; ++i)
        if (out[i - lo] != m.s[hi - 1 - (i - lo)]) ++bad;

    uint64_t total_bad = bclx::allreduce(bad, BCL::sum<uint64_t>{});
    if (me == 0)
        printf("<- reversed (distributed Treiber stack, %s)\n",
               total_bad == 0 ? "verified" : "MISMATCH!");

    BCL::finalize();
    return 0;
}
