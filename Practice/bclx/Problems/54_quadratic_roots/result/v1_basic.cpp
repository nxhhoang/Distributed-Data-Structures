// 54 — Finding the Roots of a Quadratic Equation | v1: broadcast 3 doubles
// ax^2 + bx + c = 0. Handles the degenerate linear case (a == 0), the double
// root (D == 0), two real roots (D > 0) and the complex pair (D < 0).
// Run: make run PROB=54_quadratic_roots SRC=result/v1_basic.cpp NP=4 ARGS="1 -5 6"
// (roots 3 and 2; try ARGS="1 2 5" for the complex case)

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <bclx/bclx.hpp>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "usage: %s <a> <b> <c>\n", argv[0]);
        return 1;
    }

    BCL::init();

    double a = 0, b = 0, c = 0;
    if (BCL::rank() == 0) {
        a = atof(argv[1]);
        b = atof(argv[2]);
        c = atof(argv[3]);
    }
    a = BCL::broadcast(a, 0);
    b = BCL::broadcast(b, 0);
    c = BCL::broadcast(c, 0);

    if (BCL::rank() == 0) {
        printf("equation: %.6f x^2 + %.6f x + %.6f = 0\n", a, b, c);

        if (fabs(a) < 1e-12) {
            if (fabs(b) < 1e-12)
                printf("degenerate: no (or infinitely many) roots\n");
            else
                printf("linear, root = %.6f\n", -c / b);
        } else {
            double disc = b * b - 4 * a * c;
            if (disc > 0) {
                double sq = sqrt(disc);
                printf("two real roots: %.6f and %.6f\n",
                       (-b + sq) / (2 * a), (-b - sq) / (2 * a));
            } else if (disc == 0) {
                printf("double root: %.6f\n", -b / (2 * a));
            } else {
                double sq = sqrt(-disc);
                printf("complex roots: %.6f + %.6fi and %.6f - %.6fi\n",
                       -b / (2 * a), sq / (2 * a), -b / (2 * a), sq / (2 * a));
            }
        }
    }

    BCL::finalize();
    return 0;
}
