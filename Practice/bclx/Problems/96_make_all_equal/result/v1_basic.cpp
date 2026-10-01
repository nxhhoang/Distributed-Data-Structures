// 96 — Determine Whether All Numbers of an Array Can Be Made Equal | v1
// Allowed operations: multiply any element by 2 or by 3 any number of times.
// Then two numbers can meet iff they are equal after stripping all factors
// of 2 and 3 — so the whole array can be made equal iff every stripped
// remainder is the same.
// Run: make run PROB=96_make_all_equal SRC=result/v1_basic.cpp NP=4 ARGS="8"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 100 + 1;
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
        std::vector<uint64_t> a(n);
        for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

        bool equal = true;
        uint64_t r0 = 0;
        for (uint64_t i = 0; i < n; ++i) {
            uint64_t v = a[i];
            while (v % 2 == 0) v /= 2;
            while (v % 3 == 0) v /= 3;
            if (i == 0) r0 = v;
            else if (v != r0) { equal = false; break; }
        }

        printf("all %llu numbers can%s be made equal (x2/x3 multiplications)\n",
               n, equal ? "" : "NOT");
    }

    BCL::finalize();
    return 0;
}
