// 117 — Remove Brackets from an Algebraic Expression | v1: filter ()[]{}
// Run: make run PROB=117_remove_brackets SRC=result/v1_basic.cpp NP=4 ARGS="(a+b)*[c-d]/{e*(f+g)}"

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
        fprintf(stderr, "usage: %s <expression>\n", argv[0]);
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
    for (uint64_t i = 0; i < m.len; ++i) {
        char c = m.s[i];
        if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}')
            continue;
        out[k++] = c;
    }
    out[k] = '\0';

    if (BCL::rank() == 0)
        printf("brackets removed: \"%s\"\n", out);

    BCL::finalize();
    return 0;
}
