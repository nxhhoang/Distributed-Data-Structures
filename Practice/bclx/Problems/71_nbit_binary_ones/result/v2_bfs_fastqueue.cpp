// 71 — All N-bit Binary Numbers With 1's >= 0's in Every Prefix | v2: BFS on FastQueue
// Same level-synchronized BFS as 68/v2, on the distributed FastQueue:
// a '1' may always be appended; a '0' only while ones > zeros currently
// holds. The cross-check counts the same set with a local DFS.
// Run: make run PROB=71_nbit_binary_ones SRC=result/v2_bfs_fastqueue.cpp NP=4 ARGS="3"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <bclx/bclx.hpp>
#include <bcl/containers/FastQueue.hpp>

static uint64_t count_dfs(uint64_t pos, uint64_t n, uint64_t ones, uint64_t zeros) {
    if (pos == n) return 1;
    uint64_t c = count_dfs(pos + 1, n, ones + 1, zeros);   // append '1'
    if (ones > zeros)
        c += count_dfs(pos + 1, n, ones, zeros + 1);       // append '0'
    return c;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n bits>\n", argv[0]);
        return 1;
    }

    BCL::init();
    uint64_t P = BCL::nprocs(), me = BCL::rank();

    uint64_t n = 0;
    if (me == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n == 0) n = 1;
    if (n > 14) n = 14;

    BCL::FastQueue<std::string> q(0, 1u << 16);

    if (me == 0) q.push(std::string(""));
    bclx::barrier_sync();

    std::vector<std::string> results;
    while (true) {
        uint64_t count = q.size();
        if (count == 0) break;

        uint64_t my_lo = count * me / P, my_hi = count * (me + 1) / P;
        for (uint64_t i = my_lo; i < my_hi; ++i) {
            std::string s;
            if (!q.pop(s)) break;

            if (s.size() == n) {
                results.push_back(s);
                continue;
            }
            uint64_t ones = 0;
            for (char c : s) if (c == '1') ++ones;
            uint64_t zeros = s.size() - ones;

            q.push(s + "1");                    // '1' is always allowed
            if (ones > zeros) q.push(s + "0");  // '0' keeps the prefix valid
        }
        bclx::barrier_sync();
    }

    for (const std::string &s : results)
        printf("[rank %llu] %s\n", me, s.c_str());

    uint64_t total = bclx::allreduce(results.size(), BCL::sum<uint64_t>{});
    if (me == 0) {
        uint64_t expected = count_dfs(0, n, 0, 0);
        printf("total = %llu (DFS cross-check = %llu -> %s)\n",
               total, expected, total == expected ? "MATCH" : "MISMATCH!");
    }

    BCL::finalize();
    return 0;
}
