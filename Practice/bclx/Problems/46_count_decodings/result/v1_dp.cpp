// 46 — Count Possible Decodings of a Given Digit Sequence | v1: DP on rank 0
// "121" decodes as ABA, AU, LA -> 3. The recurrence is Fibonacci-shaped
// (dp[i] = dp[i-1] + dp[i-2] when the two-digit window 10..26 is valid), so
// the computation is inherently sequential — group C. The string rides in
// the fixed-buffer struct for the broadcast.
// Run: make run PROB=46_count_decodings SRC=result/v1_dp.cpp NP=4 ARGS="121"

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
    if (argc < 2) {
        fprintf(stderr, "usage: %s <digit sequence>\n", argv[0]);
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
        if (m.len == 0 || m.s[0] == '0') {
            printf("decodings of \"%s\": 0\n", m.s);
        } else {
            std::vector<uint64_t> dp(m.len + 1);
            dp[0] = 1;
            dp[1] = 1;
            for (uint64_t i = 2; i <= m.len; ++i) {
                dp[i] = 0;
                if (m.s[i - 1] != '0')                       // single digit
                    dp[i] += dp[i - 1];
                uint64_t two = (uint64_t)(m.s[i - 2] - '0') * 10 + (uint64_t)(m.s[i - 1] - '0');
                if (two >= 10 && two <= 26)                 // two-digit window
                    dp[i] += dp[i - 2];
            }
            printf("decodings of \"%s\": %llu\n", m.s, dp[m.len]);
        }
    }

    BCL::finalize();
    return 0;
}
