/* Testing complex.h */
#define UNITTEST_IMPLEMENTATION
#include "unittest.h"

#include <math.h>
#include <stdio.h>

#define COMPLEX_IMPLEMENTATION
#include "complex.h"

static int close(double a, double b);
static int complex_equal(complex_t a, complex_t b);
static int complex_close(complex_t a, complex_t b);

static char *test_complex_real(void);
static char *test_complex_imag(void);
static char *test_complex_abs(void);
static char *test_complex_arg(void);

static char *test_complex_add(void);
static char *test_complex_sub(void);
static char *test_complex_mul(void);
static char *test_complex_div(void);

static char *test_all(void) {
    unit_test(test_complex_real);
    unit_test(test_complex_imag);
    unit_test(test_complex_abs);
    unit_test(test_complex_arg);

    unit_test(test_complex_add);
    unit_test(test_complex_sub);
    unit_test(test_complex_mul);
    unit_test(test_complex_div);
    return 0;
}



int main(void) {
    char *result = test_all();
    if (result != 0)
        printf("%s\n", result);
    else
        printf("\x1b[32mALL TESTS PASSED\x1b[0m\n");

    printf("%d tests run\n", UNIT_TEST_COUNT);
    
    return result != 0;
}



static int close(double a, double b) {
    return abs(a-b) < 1e-8; 
}

static int complex_equal(complex_t a, complex_t b) {
    return complex_real(a) == complex_real(b) && complex_imag(a) == complex_imag(b);
}

static int complex_close(complex_t a, complex_t b) {
    return close(complex_real(a), complex_real(b)) && close(complex_imag(a), complex_imag(b));
}

static char *test_complex_real(void) {
    double x = 1.0;
    double y = 2.0;
    complex_t z = {x, y};
    unit_assert("error, complex_real(z) != x", complex_real(z) == x);
    unit_assert("error, complex_real(z) != z.real", complex_real(z) == z.real);
    return 0;
}

static char *test_complex_imag(void) {
    double x = 1.0;
    double y = 2.0;
    complex_t z = {x, y};
    unit_assert("error, complex_imag(z) != y", complex_imag(z) == y);
    unit_assert("error, complex_imag(z) != z.imag", complex_imag(z) == z.imag);
    return 0;
}

static char *test_complex_abs(void) {
    double x = 1.0;
    double y = 2.0;
    complex_t z = {x, y};
    unit_assert("error, complex_abs(z) != sqrt(x*x + y*y)", complex_abs(z) == sqrt(x*x + y*y));
}

static char *test_complex_arg(void) {
    double x = 1.0;
    double y = 2.0;
    complex_t z = {x, y};
    unit_assert("error, complex_arg(z) != atan2(y,x)", complex_arg(z) == atan2(y,x));
}

static char *test_complex_add(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t z2 = {3.0, 1.0};
    complex_t res = {2.0, 3.0};
    unit_assert("error, complex_add(z1, z2) != {2.0, 3.0}\n",
                complex_close(complex_add(z1, z2), res));
}

static char *test_complex_sub(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t z2 = {3.0, 1.0};
    complex_t res = {-4.0, 1.0};
    unit_assert("error, complex_sub(z1, z2) != {-4.0, 1.0}\n",
                complex_close(complex_sub(z1, z2), res));
}

static char *test_complex_mul(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t z2 = {3.0, 1.0};
    complex_t res = {-5.0, 5.0};
    unit_assert("error, complex_mul(z1, z2) != {-5.0, 5.0}\n",
                complex_close(complex_mul(z1, z2), res));
}

static char *test_complex_div(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t z2 = {3.0, 1.0};
    complex_t res = {-0.1, 0.7};
    unit_assert("error, complex_div(z1, z2) != {-0.1, 0.7}\n",
                complex_close(complex_div(z1, z2), res));
}
