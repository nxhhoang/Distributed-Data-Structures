// 143 — Find the Factorial of a Large Number | v1: digit-array multiplication
// The result does not fit any primitive type, so it lives as a vector of
// decimal digits (least significant first); multiply by 2, 3, ..., n, one
// digit at a time with carry. 18/69 capped at 20! — this version goes beyond.
// Run: make run PROB=143_factorial_big SRC=result/v1_basic.cpp NP=4 ARGS="100"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t n = 0;
    if (BCL::rank() == 0) n = strtoull(argv[1], nullptr, 10);
    n = BCL::broadcast(n, 0);
    if (n > 500) n = 500;

    if (BCL::rank() == 0) {
        std::vector<uint64_t> digits = {1};   // least significant digit first

        for (uint64_t m = 2; m <= n; ++m) {
            uint64_t carry = 0;
            for (size_t i = 0; i < digits.size(); ++i) {
                uint64_t prod = digits[i] * m + carry;
                digits[i] = prod % 10;
                carry = prod / 10;
            }
            while (carry > 0) {
                digits.push_back(carry % 10);
                carry /= 10;
            }
        }

        printf("%llu! has %zu digits: ", n, digits.size());
        for (int64_t i = (int64_t)digits.size() - 1; i >= 0 && digits.size() - i <= 40; --i)
            putchar((char)('0' + digits[i]));
        if (digits.size() > 40) printf("...");
        printf("\n");
    }

    BCL::finalize();
    return 0;
}
