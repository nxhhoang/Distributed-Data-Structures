// 51 — Counting the Number of Days in a Given Month of a Year | v1: broadcast
// February honors the leap-year rule from problem 08.
// Run: make run PROB=51_days_in_month SRC=result/v1_basic.cpp NP=4 ARGS="2 2024"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <month 1-12> <year>\n", argv[0]);
        return 1;
    }

    BCL::init();

    uint64_t month = 0, year = 0;
    if (BCL::rank() == 0) {
        month = strtoull(argv[1], nullptr, 10);
        year = strtoull(argv[2], nullptr, 10);
    }
    month = BCL::broadcast(month, 0);
    year = BCL::broadcast(year, 0);

    if (BCL::rank() == 0) {
        if (month < 1 || month > 12) {
            fprintf(stderr, "month must be 1..12\n");
        } else {
            static const int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            uint64_t d = days[month - 1];
            bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            if (month == 2 && leap) d = 29;
            printf("month %llu of %llu has %llu days\n", month, year, d);
        }
    }

    BCL::finalize();
    return 0;
}
