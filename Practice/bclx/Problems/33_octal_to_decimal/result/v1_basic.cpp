// 33 — Octal to Decimal conversion | v1: Horner, base 8
// Same broadcast-a-struct pattern as 32/v1. Input assumed to be valid octal.
// Run: make run PROB=33_octal_to_decimal SRC=result/v1_basic.cpp NP=4 ARGS="157"

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
        fprintf(stderr, "usage: %s <octal string>\n", argv[0]);
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
        v = v * 8 + (uint64_t)(m.s[i] - '0');

    if (BCL::rank() == 0)
        printf("octal %s = %llu decimal\n", m.s, v);

    BCL::finalize();
    return 0;
}
