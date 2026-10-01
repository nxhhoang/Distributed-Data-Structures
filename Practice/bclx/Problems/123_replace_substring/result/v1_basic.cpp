// 123 — Replace a Sub-string in a String | v1: find + rebuild (all occurrences)
// Run: make run PROB=123_replace_substring SRC=result/v1_basic.cpp NP=4 ARGS="the cat sat on the mat the end the now the" "the" "a"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    char sub[40];
    char rep[40];
    uint64_t len, sublen, replen;
};

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "usage: %s <string> <old substring> <new substring>\n", argv[0]);
        return 1;
    }

    BCL::init();

    str_msg m{};
    m.len = m.sublen = m.replen = 0;
    if (BCL::rank() == 0) {
        strncpy(m.s, argv[1], sizeof(m.s) - 1);
        strncpy(m.sub, argv[2], sizeof(m.sub) - 1);
        strncpy(m.rep, argv[3], sizeof(m.rep) - 1);
        m.len = strlen(m.s);
        m.sublen = strlen(m.sub);
        m.replen = strlen(m.rep);
    }
    m = BCL::broadcast(m, 0);

    if (BCL::rank() == 0) {
        if (m.sublen == 0) {
            printf("empty substring — nothing to replace\n");
        } else {
            char out[160];
            uint64_t k = 0, i = 0, replaced = 0;
            while (i < m.len) {
                if (i + m.sublen <= m.len &&
                    strncmp(m.s + i, m.sub, m.sublen) == 0) {
                    for (uint64_t j = 0; j < m.replen; ++j) out[k++] = m.rep[j];
                    i += m.sublen;
                    ++replaced;
                } else {
                    out[k++] = m.s[i++];
                }
            }
            out[k] = '\0';
            printf("replaced %llu occurrence(s): \"%s\"\n", replaced, out);
        }
    }

    BCL::finalize();
    return 0;
}
