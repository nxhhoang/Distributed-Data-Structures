// 109 — Length of the String Without Using strlen() | v1: loop until '\0'
// (62 is the recursive twin of this count.)
// Run: make run PROB=109_strlen_manual SRC=result/v1_basic.cpp NP=4 ARGS="hello world"

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
        m.len = strlen(m.s);   // used only to fill the struct, not for the answer
    }
    m = BCL::broadcast(m, 0);

    uint64_t len = 0;
    while (m.s[len] != '\0') ++len;   // the manual count

    if (BCL::rank() == 0)
        printf("length of \"%s\" = %llu\n", m.s, len);

    BCL::finalize();
    return 0;
}
