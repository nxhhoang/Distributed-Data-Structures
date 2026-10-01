// 125 — Count Common Sub-Sequences in Two Strings | v1: DP with inclusion-exclusion
// dp[i][j] = number of common subsequences of A[0..i) and B[0..j):
//   dp[i][j] = dp[i-1][j] + dp[i][j-1] - dp[i-1][j-1]
//   + (A[i-1] == B[j-1] ? dp[i-1][j-1] + 1 : 0)
// Inherently sequential (each cell needs its neighbors) — group C.
// Run: make run PROB=125_common_subsequence_count SRC=result/v1_basic.cpp NP=4 ARGS="ajblqcpzg apple"

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

    if (BCL::rank() == 0) {
        uint64_t n = a.len, m = b.len;
        std::vector<std::vector<uint64_t>> dp(n + 1, std::vector<uint64_t>(m + 1, 0));

        for (uint64_t i = 1; i <= n; ++i) {
            for (uint64_t j = 1; j <= m; ++j) {
                uint64_t v = dp[i - 1][j] + dp[i][j - 1];
                v -= (v >= dp[i - 1][j - 1]) ? dp[i - 1][j - 1]
                                             : v;   // avoid unsigned underflow
                if (a.s[i - 1] == b.s[j - 1]) v += dp[i - 1][j - 1] + 1;
                dp[i][j] = v;
            }
        }

        printf("common subsequences of \"%s\" and \"%s\": %llu\n",
               a.s, b.s, dp[n][m]);
    }

    BCL::finalize();
    return 0;
}
