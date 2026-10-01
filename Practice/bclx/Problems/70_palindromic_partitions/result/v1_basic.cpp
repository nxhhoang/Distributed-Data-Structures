// 70 — Find All Possible Palindromic Partitions of a String | v1: backtracking
// At each start position, try every cut end that yields a palindrome; when the
// cut reaches the string's end, print the partition.
// Run: make run PROB=70_palindromic_partitions SRC=result/v1_basic.cpp NP=4 ARGS="aab"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    uint64_t len;
};

static bool is_pal(const char *s, size_t l, size_t r) {
    while (l < r) {
        if (s[l] != s[r]) return false;
        ++l;
        --r;
    }
    return true;
}

static void part(const char *s, size_t n, size_t start,
                 std::vector<std::string> &path) {
    if (start == n) {
        for (size_t i = 0; i < path.size(); ++i)
            printf(i ? " %s" : "%s", path[i].c_str());
        printf("\n");
        return;
    }
    for (size_t end = start; end < n; ++end) {
        if (is_pal(s, start, end)) {
            path.push_back(std::string(s + start, end - start + 1));
            part(s, n, end + 1, path);
            path.pop_back();   // backtrack
        }
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
    if (m.len > 12) m.len = 12;

    if (BCL::rank() == 0) {
        std::vector<std::string> path;
        part(m.s, m.len, 0, path);
    }

    BCL::finalize();
    return 0;
}
