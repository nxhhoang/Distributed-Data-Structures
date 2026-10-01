// 114 — Print the Given String in Reverse Order | v1: backwards print
// Run: make run PROB=114_reverse_string SRC=result/v1_basic.cpp NP=4 ARGS="hello world"

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

    if (BCL::rank() == 0) {
        printf("\"%s\" reversed = \"", m.s);
        for (int64_t i = (int64_t)m.len - 1; i >= 0; --i) putchar(m.s[i]);
        printf("\"\n");
    }

    BCL::finalize();
    return 0;
}
