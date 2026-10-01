// 86 — Finding Repeating Elements in an Array | v1: freq map, count > 1
// Run: make run PROB=86_repeating_elements SRC=result/v1_basic.cpp NP=4 ARGS="30"

#include <cstdio>
#include <cstdlib>
#include <map>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n == 0) n = 1;

    if (BCL::rank() == 0) {
        std::map<uint64_t, uint64_t> freq;
        for (uint64_t i = 0; i < n; ++i) ++freq[value_at(i)];

        printf("repeating elements: ");
        for (const auto &kv : freq)
            if (kv.second > 1) printf("%llu(%llux) ", kv.first, kv.second);
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
