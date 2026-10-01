// 40 — Quadrants in Which a Given Coordinate Lies | v1: broadcast + sign check
// Also handles the axes and the origin.
// Run: make run PROB=40_quadrants SRC=result/v1_basic.cpp NP=4 ARGS="-3 5"

#include <cstdio>
#include <cstdlib>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <x> <y>\n", argv[0]);
        return 1;
    }

    BCL::init();

    long long x = 0, y = 0;
    if (BCL::rank() == 0) {
        x = atoll(argv[1]);
        y = atoll(argv[2]);
    }
    x = BCL::broadcast(x, 0);
    y = BCL::broadcast(y, 0);

    const char *where;
    if (x == 0 && y == 0)      where = "the origin";
    else if (x == 0)           where = "the Y-axis";
    else if (y == 0)           where = "the X-axis";
    else if (x > 0 && y > 0)   where = "Quadrant 1";
    else if (x < 0 && y > 0)   where = "Quadrant 2";
    else if (x < 0 && y < 0)   where = "Quadrant 3";
    else                       where = "Quadrant 4";

    if (BCL::rank() == 0)
        printf("(%lld, %lld) lies in %s\n", x, y, where);

    BCL::finalize();
    return 0;
}
