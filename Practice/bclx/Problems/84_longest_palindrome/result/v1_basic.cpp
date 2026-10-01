// 84 — Finding the Longest Palindrome in an Array | v1: per-element check
// Each element is tested as a number (its decimal digits); the answer is the
// one with the MOST digits, ties broken by the larger value. Values are
// generated below 150 so 3-digit palindromes (101, 111, ...) can appear.
// Run: make run PROB=84_longest_palindrome SRC=result/v1_basic.cpp NP=4 ARGS="60"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

static uint64_t value_at(uint64_t i) {
    return (i * 2654435761ull + 17) % 150;
}

static uint64_t digits_of(uint64_t v) {
    uint64_t d = 1;
    for (; v >= 10; v /= 10) ++d;
    return d;
}

static bool is_num_pal(uint64_t v) {
    uint64_t rev = 0, x = v;
    for (; x > 0; x /= 10) rev = rev * 10 + x % 10;
    return v == rev;
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

    std::vector<uint64_t> a(n);
    for (uint64_t i = 0; i < n; ++i) a[i] = value_at(i);

    bool found = false;
    uint64_t best = 0;
    for (uint64_t i = 0; i < n; ++i) {
        if (!is_num_pal(a[i])) continue;
        if (!found || digits_of(a[i]) > digits_of(best) ||
            (digits_of(a[i]) == digits_of(best) && a[i] > best)) {
            best = a[i];
            found = true;
        }
    }

    if (BCL::rank() == 0) {
        if (found) printf("longest palindrome in array = %llu (%llu digits)\n",
                          best, digits_of(best));
        else printf("no palindromic element in the array\n");
    }

    BCL::finalize();
    return 0;
}
