// 106 — Check Whether a Character Is a Vowel or Consonant | v1: broadcast a char
// Run: make run PROB=106_vowel_or_consonant SRC=result/v1_basic.cpp NP=4 ARGS="e"

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 2 || argv[1][0] == '\0') {
        fprintf(stderr, "usage: %s <character>\n", argv[0]);
        return 1;
    }

    BCL::init();

    char c = 0;
    if (BCL::rank() == 0) c = argv[1][0];
    c = BCL::broadcast(c, 0);   // chars are trivially broadcastable

    bool vowel = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
                  c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');

    if (BCL::rank() == 0) {
        if (vowel)
            printf("'%c' is a vowel\n", c);
        else if (std::isalpha((unsigned char)c))
            printf("'%c' is a consonant\n", c);
        else
            printf("'%c' is neither a vowel nor a consonant (not an alphabet)\n", c);
    }

    BCL::finalize();
    return 0;
}
