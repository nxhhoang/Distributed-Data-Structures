// 115 — Remove All Characters from a String Except Alphabets | v1: isalpha filter
// Run: make run PROB=115_keep_alphabets_only SRC=result/v1_basic.cpp NP=4 ARGS="Hi, 2024 World!"

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

    char out[80];
    uint64_t k = 0;
    for (uint64_t i = 0; i < m.len; ++i)
        if (std::isalpha((unsigned char)m.s[i])) out[k++] = m.s[i];
    out[k] = '\0';

    if (BCL::rank() == 0)
        printf("alphabets only: \"%s\"\n", out);

    BCL::finalize();
    return 0;
}
