// 112 — Remove the Vowels from a String | v1: filter
// Run: make run PROB=112_remove_vowels SRC=result/v1_basic.cpp NP=4 ARGS="beautiful day"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    uint64_t len;
};

static bool is_vowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
           c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U';
}

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

    char out[80];
    uint64_t k = 0;
    for (uint64_t i = 0; i < m.len; ++i)
        if (!is_vowel(m.s[i])) out[k++] = m.s[i];
    out[k] = '\0';

    if (BCL::rank() == 0)
        printf("vowels removed: \"%s\"\n", out);

    BCL::finalize();
    return 0;
}
