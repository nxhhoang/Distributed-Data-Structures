// 38 — Binary to Octal conversion | v1: pad to a multiple of 3, group bits
// Each group of 3 bits maps directly to one octal digit.
// Run: make run PROB=38_binary_to_octal SRC=result/v1_basic.cpp NP=4 ARGS="101101"

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

    str_msg m{};
    m.len = 0;
    if (BCL::rank() == 0) {
        strncpy(m.s, argv[1], sizeof(m.s) - 1);
        m.len = strlen(m.s);
    }
    m = BCL::broadcast(m, 0);

    // left-pad with '0' until the length is a multiple of 3
    uint64_t pad = (3 - m.len % 3) % 3;
    char p[96];
    for (uint64_t i = 0; i < pad; ++i) p[i] = '0';
    for (uint64_t i = 0; i < m.len; ++i) p[pad + i] = m.s[i];
    uint64_t plen = pad + m.len;

    char out[40];
    uint64_t k = 0;
    for (uint64_t i = 0; i < plen; i += 3)
        out[k++] = (char)('0' + (p[i] - '0') * 4 + (p[i + 1] - '0') * 2 + (p[i + 2] - '0'));

    if (BCL::rank() == 0) {
        printf("binary %s in octal = ", m.s);
        for (uint64_t i = 0; i < k; ++i) putchar(out[i]);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
