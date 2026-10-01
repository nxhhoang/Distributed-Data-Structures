// 122 — Check if Two Strings Are Anagrams or Not | v1: histogram compare
// Case-insensitive over the 26 letters.
// Run: make run PROB=122_anagram_check SRC=result/v1_basic.cpp NP=4 ARGS="listen silent"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    uint64_t len;
};

static void count_letters(const char *s, uint64_t n, uint64_t freq[26]) {
    for (uint64_t i = 0; i < n; ++i) {
        unsigned char c = (unsigned char)std::tolower((unsigned char)s[i]);
        if (c >= 'a' && c <= 'z') ++freq[c - 'a'];
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <string A> <string B>\n", argv[0]);
        return 1;
    }

    BCL::init();

    str_msg a{}, b{};
    a.len = b.len = 0;
    if (BCL::rank() == 0) {
        strncpy(a.s, argv[1], sizeof(a.s) - 1);
        a.len = strlen(a.s);
        strncpy(b.s, argv[2], sizeof(b.s) - 1);
        b.len = strlen(b.s);
    }
    a = BCL::broadcast(a, 0);
    b = BCL::broadcast(b, 0);

    uint64_t fa[26] = {0}, fb[26] = {0};
    count_letters(a.s, a.len, fa);
    count_letters(b.s, b.len, fb);

    bool anagram = true;
    for (int c = 0; c < 26 && anagram; ++c)
        if (fa[c] != fb[c]) anagram = false;

    if (BCL::rank() == 0)
        printf("\"%s\" and \"%s\" are %sanagrams\n", a.s, b.s, anagram ? "" : "NOT ");

    BCL::finalize();
    return 0;
}
