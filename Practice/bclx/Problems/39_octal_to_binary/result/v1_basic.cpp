// 39 — Octal to Binary conversion | v1: each octal digit -> 3 bits
// Leading zeros of the first group are stripped (but at least one digit).
// Run: make run PROB=39_octal_to_binary SRC=result/v1_basic.cpp NP=4 ARGS="345"

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

    char out[300];
    uint64_t k = 0;
    for (uint64_t j = 0; j < m.len; ++j) {
        int d = m.s[j] - '0';
        out[k++] = (char)('0' + ((d >> 2) & 1));
        out[k++] = (char)('0' + ((d >> 1) & 1));
        out[k++] = (char)('0' + (d & 1));
    }

    // strip leading zeros, keep at least one digit
    uint64_t start = 0;
    while (start + 1 < k && out[start] == '0') ++start;

    if (BCL::rank() == 0) {
        printf("octal %s in binary = ", m.s);
        for (uint64_t i = start; i < k; ++i) putchar(out[i]);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
