// 119 — Capitalize the First and Last Character of Each Word | v1
// A word is a maximal run of non-space characters; single-letter words are
// capitalized once.
// Run: make run PROB=119_capitalize_word_ends SRC=result/v1_basic.cpp NP=4 ARGS="hello brave new world"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <bclx/bclx.hpp>

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

    str_msg m{};
    m.len = 0;
    if (BCL::rank() == 0) {
        strncpy(m.s, argv[1], sizeof(m.s) - 1);
        m.len = strlen(m.s);
    }
    m = BCL::broadcast(m, 0);

    uint64_t i = 0;
    while (i < m.len) {
        while (i < m.len && m.s[i] == ' ') ++i;   // skip spaces
        if (i >= m.len) break;
        uint64_t start = i;
        while (i < m.len && m.s[i] != ' ') ++i;   // consume the word
        uint64_t end = i - 1;

        m.s[start] = (char)std::toupper((unsigned char)m.s[start]);
        if (end > start)
            m.s[end] = (char)std::toupper((unsigned char)m.s[end]);
    }

    if (BCL::rank() == 0)
        printf("word ends capitalized: \"%s\"\n", m.s);

    BCL::finalize();
    return 0;
}
