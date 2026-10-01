// 73 — Remove All Adjacent Duplicate Characters Recursively | v1: run-collapse
// Find the first run of >= 2 equal adjacent characters, delete the WHOLE run,
// and recurse on the collapsed string until stable.
// e.g. "abssbe" -> "abbe" -> "ae".
// Run: make run PROB=73_remove_adjacent_duplicates SRC=result/v1_basic.cpp NP=4 ARGS="abssbe"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    uint64_t len;
};

static std::string rmdup(const std::string &s) {
    for (size_t i = 0; i < s.size(); ) {
        size_t j = i;
        while (j < s.size() && s[j] == s[i]) ++j;   // run [i, j)
        if (j - i >= 2) {
            std::string t = s.substr(0, i) + s.substr(j);
            return rmdup(t);   // recurse on the collapsed string
        }
        i = j;
    }
    return s;   // no adjacent run left
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

    if (BCL::rank() == 0) {
        m.s[m.len] = '\0';
        printf("\"%s\" -> \"%s\" (recursive run-collapse)\n", m.s,
               rmdup(std::string(m.s)).c_str());
    }

    BCL::finalize();
    return 0;
}
