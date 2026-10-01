// 108 — Find the ASCII Value of a Character | v1: broadcast a char
// Run: make run PROB=108_ascii_value SRC=result/v1_basic.cpp NP=4 ARGS="A"

#include <cstdio>
#include <cstdlib>
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

    if (BCL::rank() == 0)
        printf("ASCII value of '%c' = %d\n", c, (int)(unsigned char)c);

    BCL::finalize();
    return 0;
}
