// 118 — Count the Sum of Numbers in a String | v1: multi-digit run parsing
// Digit runs are read as whole numbers: "abc12def34gh5" -> 12 + 34 + 5 = 51.
// Run: make run PROB=118_sum_numbers_in_string SRC=result/v1_basic.cpp NP=4 ARGS="abc12def34gh5"

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

    uint64_t sum = 0, num = 0;
    bool in_number = false;
    for (uint64_t i = 0; i < m.len; ++i) {
        if (std::isdigit((unsigned char)m.s[i])) {
            num = num * 10 + (uint64_t)(m.s[i] - '0');
            in_number = true;
        } else if (in_number) {
            sum += num;
            num = 0;
            in_number = false;
        }
    }
    if (in_number) sum += num;   // a run ending at the string's end

    if (BCL::rank() == 0)
        printf("sum of numbers in \"%s\" = %llu\n", m.s, sum);

    BCL::finalize();
    return 0;
}
