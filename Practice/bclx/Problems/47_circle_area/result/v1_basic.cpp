// 47 — Calculate the Area of a Circle | v1: broadcast a double
// First appearance of broadcasting a floating-point value (trivially
// copyable, so it rides the same MPI_Bcast path).
// Run: make run PROB=47_circle_area SRC=result/v1_basic.cpp NP=4 ARGS="2.5"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <radius>\n", argv[0]);
        return 1;
    }

    BCL::init();

    double r = 0.0;
    if (BCL::rank() == 0) r = atof(argv[1]);
    r = BCL::broadcast(r, 0);

    double area = M_PI * r * r;

    if (BCL::rank() == 0)
        printf("area of circle with radius %.3f = %.6f\n", r, area);

    BCL::finalize();
    return 0;
}
