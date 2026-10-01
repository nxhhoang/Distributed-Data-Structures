// 68 — Generate All Combinations of Well-Formed Parentheses | v2: BFS on FastQueue
// The v1 backtracking becomes a level-synchronized BFS on the library's
// distributed FastQueue<std::string> (strings ride the ObjectContainer
// serialization automatically). Round structure:
//   barrier -> count = q.size() (exact) -> each rank pops its slice,
//   extends the partial strings, pushes the children -> barrier.
// Complete strings are collected locally; a Catalan cross-check verifies
// the ranks together produced exactly C_n strings.
// Run: make run PROB=68_balanced_parentheses SRC=result/v2_bfs_fastqueue.cpp NP=4 ARGS="3"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <bclx/bclx.hpp>
#include <bcl/containers/FastQueue.hpp>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <pairs n>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t n = 0;
    if (me == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n == 0) n = 1;
    if (n > 8) n = 8;

    // hosted on rank 0; wide enough for n = 8 (parents + children in flight)
    BCL::FastQueue<std::string> q(0, 1u << 17);

    if (me == 0) q.push(std::string(""));
    bclx::barrier_sync();

    std::vector<std::string> results;
    while (true) {
        uint64_t count = q.size();   // exact after the barrier
        if (count == 0) break;

        uint64_t my_lo = count * me / P, my_hi = count * (me + 1) / P;
        for (uint64_t i = my_lo; i < my_hi; ++i) {
            std::string s;
            if (!q.pop(s)) break;   // defensive; cannot happen with the slice split

            if (s.size() == 2 * n) {
                results.push_back(s);
                continue;
            }
            uint64_t open = 0;
            for (char c : s) if (c == '(') ++open;
            uint64_t close = s.size() - open;
            if (open < n) q.push(s + "(");
            if (close < open) q.push(s + ")");
        }
        bclx::barrier_sync();   // level boundary: all pushes flushed
    }

    for (const std::string &s : results)
        printf("[rank %llu] %s\n", me, s.c_str());

    uint64_t total = bclx::allreduce(results.size(), BCL::sum<uint64_t>{});
    if (me == 0) {
        // Catalan(n) = binom(2n, n) / (n+1), exact integer steps
        uint64_t binom = 1;
        for (uint64_t i = 0; i < n; ++i)
            binom = binom * (2 * n - i) / (i + 1);
        uint64_t catalan = binom / (n + 1);
        printf("total = %llu (Catalan(%llu) = %llu -> %s)\n",
               total, n, catalan, total == catalan ? "MATCH" : "MISMATCH!");
    }

    BCL::finalize();
    return 0;
}
