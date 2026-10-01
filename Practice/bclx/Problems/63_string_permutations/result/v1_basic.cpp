// 63 — Print All Permutations of a String | v1: swap recursion on rank 0
// permute(s, l, r): fix s[l] to each remaining character and recurse.
// Keep the string short — L! permutations.
// Run: make run PROB=63_string_permutations SRC=result/v1_basic.cpp NP=4 ARGS="ABC"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    uint64_t len;
};

static void permute(char *s, int l, int r) {
    if (l == r) {
        printf("%s\n", s);
        return;
    }
    for (int i = l; i <= r; ++i) {
        std::swap(s[l], s[i]);
        permute(s, l + 1, r);
        std::swap(s[l], s[i]);   // backtrack
    }
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
    if (m.len > 8) m.len = 8;   // keep the output sane

    if (BCL::rank() == 0) {
        m.s[m.len] = '\0';
        permute(m.s, 0, (int)m.len - 1);
    }

    BCL::finalize();
    return 0;
}
