// 126 — Check if Two Strings Match Where One Contains Wildcard Characters | v1: DP
// '?' matches exactly one character, '*' matches any sequence (including
// empty). dp[i][j] = does S[0..i) match P[0..j)?
// Run: make run PROB=126_wildcard_match SRC=result/v1_basic.cpp NP=4 ARGS="distributedmemory dist*memory"
// (also try: "hello he?lo", "world w*d", "abc *c*")

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    uint64_t len;
};

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <string> <pattern with ? and *>\n", argv[0]);
        return 1;
    }

    BCL::init();

    str_msg s{}, p{};
    s.len = p.len = 0;
    if (BCL::rank() == 0) {
        strncpy(s.s, argv[1], sizeof(s.s) - 1);
        s.len = strlen(s.s);
        strncpy(p.s, argv[2], sizeof(p.s) - 1);
        p.len = strlen(p.s);
    }
    s = BCL::broadcast(s, 0);
    p = BCL::broadcast(p, 0);

    if (BCL::rank() == 0) {
        uint64_t n = s.len, m = p.len;
        std::vector<std::vector<bool>> dp(n + 1, std::vector<bool>(m + 1, false));

        dp[0][0] = true;
        for (uint64_t j = 1; j <= m; ++j)                      // empty string vs pattern
            dp[0][j] = dp[0][j - 1] && p.s[j - 1] == '*';

        for (uint64_t i = 1; i <= n; ++i) {
            for (uint64_t j = 1; j <= m; ++j) {
                if (p.s[j - 1] == '*')
                    dp[i][j] = dp[i - 1][j] || dp[i][j - 1];   // consume one / none
                else if (p.s[j - 1] == '?' || p.s[j - 1] == s.s[i - 1])
                    dp[i][j] = dp[i - 1][j - 1];
            }
        }

        printf("\"%s\" %s pattern \"%s\"\n",
               s.s, dp[n][m] ? "MATCHES" : "does NOT match", p.s);
    }

    BCL::finalize();
    return 0;
}
