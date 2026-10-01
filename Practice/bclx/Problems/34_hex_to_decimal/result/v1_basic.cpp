// 34 — Hexadecimal to Decimal conversion | v1: Horner, base 16
// Parses 0-9, a-f, A-F. Same broadcast-a-struct pattern as 32/v1.
// Run: make run PROB=34_hex_to_decimal SRC=result/v1_basic.cpp NP=4 ARGS="1A3F"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    uint64_t len;
};

static int hex_val(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <hex string>\n", argv[0]);
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
    for (uint64_t i = 0; i < m.len; ++i) {
        int d = hex_val(m.s[i]);
        if (d < 0) {
            if (BCL::rank() == 0) fprintf(stderr, "invalid hex digit '%c'\n", m.s[i]);
            BCL::finalize();
            return 1;
        }
        v = v * 16 + (uint64_t)d;
    }

    if (BCL::rank() == 0)
        printf("hex %s = %llu decimal\n", m.s, v);

    BCL::finalize();
    return 0;
}
