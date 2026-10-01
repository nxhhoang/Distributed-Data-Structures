// 62 — Length of a String Using Recursion | v1: len(s) = 1 + len(s+1)
// The string rides the fixed-buffer struct for the broadcast (see 32/v1).
// Run: make run PROB=62_recursive_strlen SRC=result/v1_basic.cpp NP=4 ARGS="recursion"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    uint64_t len;
};

static uint64_t len_rec(const char *s) {
    return *s == '\0' ? 0 : 1 + len_rec(s + 1);
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

    if (BCL::rank() == 0)
        printf("length of \"%s\" = %llu (recursion)\n", m.s, len_rec(m.s));

    BCL::finalize();
    return 0;
}
