// 120 — Calculate the Frequency of Characters in a String | v1: 26-bucket table
// Case-insensitive over the 26 English letters; other characters are ignored.
// Run: make run PROB=120_char_frequency SRC=result/v1_basic.cpp NP=4 ARGS="mississippi"

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

    uint64_t freq[26] = {0};
    for (uint64_t i = 0; i < m.len; ++i) {
        unsigned char c = (unsigned char)std::tolower((unsigned char)m.s[i]);
        if (c >= 'a' && c <= 'z') ++freq[c - 'a'];
    }

    if (BCL::rank() == 0) {
        printf("frequencies in \"%s\":\n", m.s);
        for (int c = 0; c < 26; ++c)
            if (freq[c] > 0)
                printf("  %c -> %llu\n", 'a' + c, freq[c]);
    }

    BCL::finalize();
    return 0;
}
