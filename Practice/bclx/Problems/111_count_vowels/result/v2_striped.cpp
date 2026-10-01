// 111 — Count the Number of Vowels | v2: striped positions + allreduce
// Each rank inspects only the characters at positions i with i % P == me
// (striping, so every rank touches characters spread over the whole string),
// and the partial vowel counts are summed with allreduce.
// Run: make run PROB=111_count_vowels SRC=result/v2_striped.cpp NP=4 ARGS="distributed memory"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    uint64_t len;
};

static bool is_vowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
           c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U';
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <string>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    str_msg m{};
    m.len = 0;
    if (me == 0) {
        strncpy(m.s, argv[1], sizeof(m.s) - 1);
        m.len = strlen(m.s);
    }
    m = BCL::broadcast(m, 0);

    uint64_t partial = 0;
    for (uint64_t i = me; i < m.len; i += P)   // my striped positions
        if (is_vowel(m.s[i])) ++partial;

    uint64_t total = bclx::allreduce(partial, BCL::sum<uint64_t>{});

    if (me == 0)
        printf("\"%s\" contains %llu vowel%s (striped over %llu ranks)\n",
               m.s, total, total == 1 ? "" : "s", P);

    BCL::finalize();
    return 0;
}
