// 107 — Check Whether a Character Is an Alphabet or Not | v1: broadcast a char
// Run: make run PROB=107_alphabet_or_not SRC=result/v1_basic.cpp NP=4 ARGS="q"

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
    c = BCL::broadcast(c, 0);

    bool alpha = std::isalpha((unsigned char)c) != 0;
    if (BCL::rank() == 0)
        printf("'%c' is %san alphabet\n", c, alpha ? "" : "NOT ");

    BCL::finalize();
    return 0;
}
