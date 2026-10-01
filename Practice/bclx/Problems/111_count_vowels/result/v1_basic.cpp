// 111 — Count the Number of Vowels | v1: single loop
// Run: make run PROB=111_count_vowels SRC=result/v1_basic.cpp NP=4 ARGS="distributed memory"

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

    uint64_t count = 0;
    for (uint64_t i = 0; i < m.len; ++i)
        if (is_vowel(m.s[i])) ++count;

    if (BCL::rank() == 0)
        printf("\"%s\" contains %llu vowel%s\n", m.s, count, count == 1 ? "" : "s");

    BCL::finalize();
    return 0;
}
