/* Testing complex.h */
#include <assert.h>
#include <math.h>
#include <stdio.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define COMPLEX_IMPLEMENTATION
#include "complex.h"

#define TESTING_EPS 1e-4

int are_close(double a, double b) {
    return abs(a-b) < TESTING_EPS; 
}

int complex_equal(complex_t a, complex_t b) {
    return complex_real(a) == complex_real(b) && complex_imag(a) == complex_imag(b);
}

int complex_are_close(complex_t a, complex_t b) {
    return are_close(complex_real(a), complex_real(b)) && are_close(complex_imag(a), complex_imag(b));
}

int main(void) {
    complex_t z = {1.0, 2.0};
    printf("\nTesting complex_real and complex_imag functions\n");
    printf(" > Input z: 1.0 + 2.0I\n");
    printf("   Real z: %.1lf (expected 1.0)\n", complex_real(z));
    assert(complex_real(z) == 1.0 && "Failure!");
    printf("   Imag z: %.1lf (expected 2.0)\n", complex_imag(z));
    assert(complex_imag(z) == 2.0 && "Failure!");
    printf(ANSI_COLOR_GREEN " * Passed\n" ANSI_COLOR_RESET);

    z = (complex_t){-1.0, 1.0};
    printf("\nTesting complex_abs and complex_arg functions\n");
    printf(" > Input z: -1.0 + 1.0I\n");
    printf("   Abs z: %.3lf (expected %.3lf)\n", complex_abs(z), sqrt(2));
    assert(are_close(complex_abs(z), sqrt(2)) && "Failure!");
    printf("   Arg z: %.3lf (expected %.3lf)\n", complex_arg(z), 3 * COMPLEX_PI / 4);
    assert(are_close(complex_arg(z), 3 * COMPLEX_PI / 4) && "Failure!");
    printf(" * Passed\n");

    printf("\nTesting complex_conj function\n"); 
    printf(" > Input z: -1.0 + 1.0I\n");
    printf("   Conj z: %.1lf - %.1lfI (expected -1.0 - 1.0I)\n", complex_real(complex_conj(z)), -complex_imag(complex_conj(z)));
    assert(complex_equal(complex_conj(z), (complex_t){-1.0, -1.0}) && "Failure!");
    printf(" * Passed\n");

    printf("\nTesting complex_proj function\n");
    printf(" > Input z: -1.0 + 1.0I\n");
    printf("   Proj z: %.1lf + %.1lfI (expected -1.0 + 1.0I)\n", complex_real(complex_proj(z)), complex_imag(complex_proj(z)));
    assert(complex_equal(complex_proj(z), z) && "Failure!");
    printf(" * Passed\n");

    z = (complex_t){INFINITY, 1.0};
    printf(" > Input z: inf + 1.0I\n");
    printf("   Proj z: %.1lf + %.1lfI (expected inf + 0.0I)\n", complex_real(complex_proj(z)), complex_imag(complex_proj(z)));
    assert(isinf(complex_real(complex_proj(z))) && "Failure!");
    assert(complex_imag(complex_proj(z)) == +0.0 && "Failure!");
    printf(" * Passed\n");

    z = (complex_t){-1.0, -INFINITY};
    printf(" > Input z: -1.0 - infI\n");
    printf("   Proj z: %.1lf - %.1lfI (expected inf - 0.0I)\n", complex_real(complex_proj(z)), -complex_imag(complex_proj(z)));
    assert(isinf(complex_real(complex_proj(z))) && "Failure!");
    assert(complex_imag(complex_proj(z)) == -0.0 && "Failure!");
    printf(" * Passed\n");

    return 0;
}
