// 110 — Toggle Each Character in a String | v1: in-place case flip
// Run: make run PROB=110_toggle_case SRC=result/v1_basic.cpp NP=4 ARGS="Hello World 42"

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

    for (uint64_t i = 0; i < m.len; ++i) {
        if (std::islower((unsigned char)m.s[i])) m.s[i] = (char)std::toupper((unsigned char)m.s[i]);
        else if (std::isupper((unsigned char)m.s[i])) m.s[i] = (char)std::tolower((unsigned char)m.s[i]);
    }

    if (BCL::rank() == 0)
        printf("toggled: \"%s\"\n", m.s);

    BCL::finalize();
    return 0;
}
