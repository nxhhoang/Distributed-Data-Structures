// 32 — Binary to Decimal conversion | v1: Horner after broadcasting the string
// Teaching point: BCL::broadcast only moves trivially-copyable types, so a
// string must ride inside a fixed-size struct (no std::string — it has a
// pointer!). Horner's rule: value = value*2 + bit.
// Run: make run PROB=32_binary_to_decimal SRC=result/v1_basic.cpp NP=4 ARGS="101101"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <bclx/bclx.hpp>

struct str_msg {          // trivially copyable -> broadcastable
    char s[80];
    uint64_t len;
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <binary string>\n", argv[0]);
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

    uint64_t v = 0;
    for (uint64_t i = 0; i < m.len; ++i)
        v = v * 2 + (uint64_t)(m.s[i] - '0');

    if (BCL::rank() == 0)
        printf("binary %s = %llu decimal\n", m.s, v);

    BCL::finalize();
    return 0;
}
