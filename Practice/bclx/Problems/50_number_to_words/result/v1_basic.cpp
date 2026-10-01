// 50 — Convert Digit/Number to Words | v1: broadcast + 3-digit groups
// Classic tutorial: split into groups of thousand / million / billion and
// spell each 3-digit group. Supports 0 .. 999,999,999,999.
// Run: make run PROB=50_number_to_words SRC=result/v1_basic.cpp NP=4 ARGS="123456789"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <bclx/bclx.hpp>

static const char *ONES[20] = {"", "one", "two", "three", "four", "five", "six",
    "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen",
    "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
static const char *TENS[10] = {"", "", "twenty", "thirty", "forty", "fifty",
    "sixty", "seventy", "eighty", "ninety"};

// spell a value < 1000
static std::string under1000(uint64_t n) {
    std::string s;
    if (n >= 100) {
        s += ONES[n / 100];
        s += " hundred";
        n %= 100;
        if (n) s += " ";
    }
    if (n >= 20) {
        s += TENS[n / 10];
        n %= 10;
        if (n) { s += " "; s += ONES[n]; }
    } else if (n > 0) {
        s += ONES[n];
    }
    return s;
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

    if (BCL::rank() == 0) {
        if (n == 0) {
            printf("zero\n");
        } else {
            const uint64_t scales[3] = {1000000000ull, 1000000ull, 1000ull};
            const char *names[3] = {"billion", "million", "thousand"};
            std::string out;
            uint64_t x = n;
            for (int i = 0; i < 3; ++i) {
                uint64_t cnt = x / scales[i];
                if (cnt > 0) {
                    if (!out.empty()) out += ", ";
                    out += under1000(cnt);
                    out += " ";
                    out += names[i];
                    x %= scales[i];
                }
            }
            if (x > 0) {
                if (!out.empty()) out += ", ";
                out += under1000(x);
            }
            printf("%llu in words: %s\n", n, out.c_str());
        }
    }

    BCL::finalize();
    return 0;
}
