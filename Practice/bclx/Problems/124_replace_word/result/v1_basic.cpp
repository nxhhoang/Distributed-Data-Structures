// 124 — Replacing a Particular Word with Another Word | v1: tokenized replace
// Words are maximal runs of non-space characters, so "the" does not match
// inside "there".
// Run: make run PROB=124_replace_word SRC=result/v1_basic.cpp NP=4 ARGS="the cat sat on the mat" "the" "a"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    char oldw[40];
    char neww[40];
    uint64_t len, oldlen, newlen;
};

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "usage: %s <string> <old word> <new word>\n", argv[0]);
        return 1;
    }

    BCL::init();

    str_msg m{};
    m.len = m.oldlen = m.newlen = 0;
    if (BCL::rank() == 0) {
        strncpy(m.s, argv[1], sizeof(m.s) - 1);
        strncpy(m.oldw, argv[2], sizeof(m.oldw) - 1);
        strncpy(m.neww, argv[3], sizeof(m.neww) - 1);
        m.len = strlen(m.s);
        m.oldlen = strlen(m.oldw);
        m.newlen = strlen(m.neww);
    }
    m = BCL::broadcast(m, 0);

    if (BCL::rank() == 0) {
        char out[160];
        uint64_t k = 0, i = 0, replaced = 0;

        while (i < m.len) {
            // copy spaces as they are
            while (i < m.len && m.s[i] == ' ') out[k++] = m.s[i++];
            if (i >= m.len) break;

            // consume one word [i, j)
            uint64_t j = i;
            while (j < m.len && m.s[j] != ' ') ++j;

            if (j - i == m.oldlen && strncmp(m.s + i, m.oldw, m.oldlen) == 0) {
                for (uint64_t t = 0; t < m.newlen; ++t) out[k++] = m.neww[t];
                ++replaced;
            } else {
                for (uint64_t t = i; t < j; ++t) out[k++] = m.s[t];
            }
            i = j;
        }
        out[k] = '\0';

        printf("replaced %llu word(s): \"%s\"\n", replaced, out);
    }

    BCL::finalize();
    return 0;
}
