// 113 — Check if the Given String Is a Palindrome or Not | v1: two pointers
// Run: make run PROB=113_string_palindrome SRC=result/v1_basic.cpp NP=4 ARGS="racecar"

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

    bool pal = true;
    for (uint64_t i = 0, j = m.len - 1; i < j && pal; ++i, --j)
        if (m.s[i] != m.s[j]) pal = false;

    if (BCL::rank() == 0)
        printf("\"%s\" is %sa palindrome\n", m.s, pal ? "" : "NOT ");

    BCL::finalize();
    return 0;
}
