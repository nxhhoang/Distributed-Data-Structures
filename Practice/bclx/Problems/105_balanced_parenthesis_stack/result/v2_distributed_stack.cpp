// 105 — Balanced Parenthesis Problem | v2: the distributed Treiber stack
// The stack lives in GLOBAL memory (dds::ts::stack from stack/inc/stack_treiber.h
// with the NMR memory manager): push/pop are one-sided RMA — remote CAS on
// the top pointer. Rank 0 drives the matching; the other ranks idle during
// the pass (parenthesis matching is inherently sequential — prefix state).
// NOTE: this file restores the missing dds scaffolding (memory/inc/sds.h).
// Run: make run PROB=105_balanced_parenthesis_stack SRC=result/v2_distributed_stack.cpp NP=4 ARGS="(()())"
// (also try "(()" and "())(")

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <bclx/bclx.hpp>

#include <memory/inc/sds.h>   // restored sequential list (dds dependency)

// minimal dds configuration (normally provided by the dds repo's config.h)
namespace dds {
    const uint64_t MASTER_UNIT = 0;
    const uint64_t TOTAL_OPS = 32768;   // memory-pool elements per rank
    std::string stack_name;
    std::string mem_manager;
    uint64_t bk_init = 2;        // backoff window (us)
    uint64_t bk_max = 1ull << 20;
}

#include <memory/inc/memory_nmr.h>     // no-reclamation memory manager
#include <stack/inc/stack_treiber.h>  // dds::ts::stack<T>

struct str_msg {
    char s[80];
    uint64_t len;
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <parenthesis string>\n", argv[0]);
        return 1;
    }

    BCL::init();

    str_msg m{};
    m.len = 0;
    if (BCL::rank() == 0) {
        strncpy(m.s, argv[1], sizeof(m.s) - 1);
        m.len = strlen(m.s);
    }
    m = BCL::broadcast(m, 0);

    // collective: allocates the global top word + per-rank memory pools
    dds::ts::stack<char> st;

    bool balanced = true;
    if (BCL::rank() == 0) {
        uint64_t depth = 0;   // tracks the stack depth to avoid empty pops
        for (uint64_t i = 0; i < m.len && balanced; ++i) {
            char c = m.s[i];
            if (c == '(') {
                st.push(c);   // remote RMA: malloc + aget + rput + cas
                ++depth;
            } else if (c == ')') {
                if (depth == 0) { balanced = false; break; }
                char top;
                st.pop(top);   // remote RMA: reserve + rget + cas
                if (top != '(') balanced = false;
                --depth;
            } else {
                balanced = false;
            }
        }
        balanced = balanced && (depth == 0);
    }

    if (BCL::rank() == 0)
        printf("\"%s\" is %sbalanced (distributed Treiber stack, NMR manager)\n",
               m.s, balanced ? "" : "NOT ");

    BCL::finalize();
    return 0;
}
