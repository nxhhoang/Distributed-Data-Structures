// 63 — All Permutations of a String | v2: striped top-level branches
// The recursion tree has L branches at the top (which character leads);
// rank me takes the branches i with i % P == me and explores them fully.
// This is THE pattern for parallelizing backtracking: stripe the first
// decision level, recurse locally below it. A count check via allreduce
// verifies that the ranks together produced exactly L! permutations.
// Run: make run PROB=63_string_permutations SRC=result/v2_striped_branches.cpp NP=3 ARGS="ABCD"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    uint64_t len;
};

static void permute_tagged(char *s, int l, int r, uint64_t me, uint64_t &count) {
    if (l == r) {
        printf("[rank %llu] %s\n", me, s);
        ++count;
        return;
    }
    for (int i = l; i <= r; ++i) {
        std::swap(s[l], s[i]);
        permute_tagged(s, l + 1, r, me, count);
        std::swap(s[l], s[i]);
    }
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
    if (m.len > 8) m.len = 8;
    m.s[m.len] = '\0';

    char buf[80];
    strncpy(buf, m.s, sizeof(buf) - 1);

    uint64_t count = 0;
    for (int i = 0; i < (int)m.len; ++i) {
        if ((uint64_t)i % P != me) continue;   // my branches of the top level only
        std::swap(buf[0], buf[i]);
        permute_tagged(buf, 1, (int)m.len - 1, me, count);
        std::swap(buf[0], buf[i]);
    }

    // verification: together the ranks must have produced L! permutations
    uint64_t total = bclx::allreduce(count, BCL::sum<uint64_t>{});
    if (me == 0) {
        uint64_t fact = 1;
        for (uint64_t i = 2; i <= m.len; ++i) fact *= i;
        printf("total permutations = %llu (expected %llu -> %s)\n",
               total, fact, total == fact ? "MATCH" : "MISMATCH!");
    }

    BCL::finalize();
    return 0;
}
