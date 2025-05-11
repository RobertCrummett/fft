/* Testing complex.h */
#include <assert.h>
#include <math.h>
#include <stdio.h>

#define COMPLEX_IMPLEMENTATION
#include "complex.h"

#define TEST_IMPLEMENTATION
#include "test.h"

int testing_close(double a, double b) {
    return abs(a-b) < 1e-8; 
}

int complex_equal(complex_t a, complex_t b) {
    return complex_real(a) == complex_real(b) && complex_imag(a) == complex_imag(b);
}

int complex_are_close(complex_t a, complex_t b) {
    return testing_close(complex_real(a), complex_real(b)) && testing_close(complex_imag(a), complex_imag(b));
}

int main(void) {
    test_suite suite = {0};
    complex_t z = {1.0, 2.0};
    test_condition(&suite, "complex_real returns expected value", complex_real(z) == z.real);
    test_condition(&suite, "complex_imag returns expected value", complex_imag(z) == z.imag);

    test_summary(&suite);
    /*
    printf("Test complex_real\n");
    printf("    Input z: 1.0 + 2.0I\n");
    printf("    Real z: %.1lf (expected 1.0)\n", complex_real(z));
    printf("   Imag z: %.1lf (expected 2.0)\n", complex_imag(z));
    testing_is_true(complex_imag(z) == 2.0);

    z = (complex_t){-1.0, 1.0};
    printf("\nTesting complex_abs and complex_arg functions\n");
    printf("    Input z: -1.0 + 1.0I\n");
    printf("    Abs z: %.3lf (expected %.3lf)\n", complex_abs(z), sqrt(2));
    assert(testing_close(complex_abs(z), sqrt(2)) && "Failure!");
    printf("    Arg z: %.3lf (expected %.3lf)\n", complex_arg(z), 3 * COMPLEX_PI / 4);
    assert(testing_close(complex_arg(z), 3 * COMPLEX_PI / 4) && "Failure!");

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
    */

    return 0;
}
