// 127 — Print All Permutations of a String in Lexicographically Sorted Order | v1
// Sort the string, then backtrack with a used[] array — iterating the
// characters in sorted order makes the output lexicographic, and skipping a
// duplicate whose identical predecessor is unused prevents repeated
// permutations. (63/v2 shows the striped distributed variant.)
// Run: make run PROB=127_permutations_sorted SRC=result/v1_basic.cpp NP=4 ARGS="aab"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    uint64_t len;
};

static void permute_sorted(const char *s, bool *used, char *out, int depth, int n) {
    if (depth == n) {
        out[depth] = '\0';
        printf("%s\n", out);
        return;
    }
    for (int i = 0; i < n; ++i) {
        if (used[i]) continue;
        // skip duplicates: same char, predecessor not used
        if (i > 0 && s[i] == s[i - 1] && !used[i - 1]) continue;

        used[i] = true;
        out[depth] = s[i];
        permute_sorted(s, used, out, depth + 1, n);
        used[i] = false;
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
        std::sort(m.s, m.s + m.len);   // sorted input -> lexicographic output

        bool used[9] = {false};
        char out[9];
        permute_sorted(m.s, used, out, 0, (int)m.len);
    }

    BCL::finalize();
    return 0;
}
