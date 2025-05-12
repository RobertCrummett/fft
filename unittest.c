/* Unit testing complex.h */
#define COMPLEX_IMPLEMENTATION
#include "complex.h"

#include <math.h>
#include <stdio.h>

#define unit_assert(message, test) do { \
    if (!(test)) { \
        printf("\x1b[31mTEST FAILED:\x1b[0m %s\n", message); \
        return message; \
    } return NULL; \
} while (0)

#define unit_test(test) do { \
    printf("Running %s...\n", #test); \
    char *message = test(); \
    UNIT_TEST_COUNT++; \
    if (message) return message; \
} while (0)

int UNIT_TEST_COUNT = 0;

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

static char *test_complex_conj(void);
static char *test_complex_proj(void);
static char *test_complex_exp(void);
static char *test_complex_log(void);
static char *test_complex_pow(void);
static char *test_complex_sqrt(void);

static char *test_complex_sin(void);
static char *test_complex_cos(void);
static char *test_complex_tan(void);

static char *test_complex_sinh(void);
static char *test_complex_cosh(void);
static char *test_complex_tanh(void);

static char *test_complex_asin(void);
static char *test_complex_acos(void);
static char *test_complex_atan(void);

static char *test_complex_asinh(void);
static char *test_complex_acosh(void);
static char *test_complex_atanh(void);

static char *test_all(void) {
    unit_test(test_complex_real);
    unit_test(test_complex_imag);
    unit_test(test_complex_abs);
    unit_test(test_complex_arg);

    unit_test(test_complex_add);
    unit_test(test_complex_sub);
    unit_test(test_complex_mul);
    unit_test(test_complex_div);

    unit_test(test_complex_conj);
    unit_test(test_complex_proj);
    unit_test(test_complex_exp);
    unit_test(test_complex_log);
    unit_test(test_complex_pow);
    unit_test(test_complex_sqrt);

    unit_test(test_complex_sin);
    unit_test(test_complex_cos);
    unit_test(test_complex_tan);

    unit_test(test_complex_sinh);
    unit_test(test_complex_cosh);
    unit_test(test_complex_tanh);

    unit_test(test_complex_asin);
    unit_test(test_complex_acos);
    unit_test(test_complex_atan);

    unit_test(test_complex_asinh);
    unit_test(test_complex_acosh);
    unit_test(test_complex_atanh);
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
    unit_assert("error, complex_add(z1, z2) != {2.0, 3.0}", complex_close(complex_add(z1, z2), res));
}

static char *test_complex_sub(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t z2 = {3.0, 1.0};
    complex_t res = {-4.0, 1.0};
    unit_assert("error, complex_sub(z1, z2) != {-4.0, 1.0}", complex_close(complex_sub(z1, z2), res));
}

static char *test_complex_mul(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t z2 = {3.0, 1.0};
    complex_t res = {-5.0, 5.0};
    unit_assert("error, complex_mul(z1, z2) != {-5.0, 5.0}", complex_close(complex_mul(z1, z2), res));
}

static char *test_complex_div(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t z2 = {3.0, 1.0};
    complex_t res = {-0.1, 0.7};
    unit_assert("error, complex_div(z1, z2) != {-0.1, 0.7}", complex_close(complex_div(z1, z2), res));
}

static char *test_complex_conj(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t res1 = {-1.0, -2.0};
    unit_assert("error, complex_conj(z1) != {-1.0, -2.0}", complex_close(complex_conj(z1), res1));
    complex_t z2 = {3.0, 4.0};
    complex_t res2 = {3.0, -4.0};
    unit_assert("error, complex_conj(z2) != {3.0, -4.0}", complex_close(complex_conj(z2), res2));
}

static char *test_complex_proj(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t r1 = {-1.0, 2.0};
    unit_assert("error, complex_proj(z1) != {-1.0, 2.0}", complex_equal(complex_proj(z1), r1));
    complex_t z2 = {INFINITY, 2.0};
    complex_t r2 = {INFINITY, +0.0};
    unit_assert("error, complex_proj(z2) != {INFINITY, +0.0}", complex_equal(complex_proj(z2), r2));
    complex_t z3 = {INFINITY, -2.0};
    complex_t r3 = {INFINITY, -0.0};
    unit_assert("error, complex_proj(z3) != {INFINITY, -0.0}", complex_equal(complex_proj(z3), r3));
    complex_t z4 = {INFINITY, INFINITY};
    complex_t r4 = {INFINITY, +0.0};
    unit_assert("error, complex_proj(z4) != {INFINITY, 0.0}", complex_equal(complex_proj(z4), r4));
}

static char *test_complex_exp(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t r1 = {-0.1530918656742263, 0.33451182923926226};
    unit_assert("error, complex_exp(z1) != {-0.1530918656742263, 0.33451182923926226}", complex_close(complex_exp(z1), r1));
}

static char *test_complex_log(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t r1 = {0.8047189562170503, 2.0344439357957027};
    unit_assert("error, complex_log(z1) != {0.8047189562170503, 2.0344439357957027}", complex_close(complex_log(z1), r1));
}

static char *test_complex_pow(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t p1 = {3.0, 1.0};
    complex_t r1 = {1.1856335688388966, 0.8551736406099939};
    unit_assert("error, complex_pow(z1, p1) != {1.1856335688388966, 0.8551736406099939}", complex_close(complex_pow(z1, p1), r1));
}

static char *test_complex_sqrt(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t r1 = {0.7861513777574233, 1.272019649514069};
    unit_assert("error, complex_sqrt(z1) != {0.7861513777574233, 1.272019649514069}", complex_close(complex_sqrt(z1), r1));
    unit_assert("error, complex_sqrt(z1) != complex_pow(z1, (complex_t){0.5, 0.0})", complex_close(complex_sqrt(z1), complex_pow(z1, (complex_t){0.5, 0.0})));
}

static char *test_complex_sin(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t r1 = {-3.165778513216168, 1.959601041421606};
    unit_assert("error, complex_sin(z1) != {-3.165778513216168, 1.959601041421606}", complex_close(complex_sin(z1), r1));
}

static char *test_complex_cos(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t r1 = {2.0327230070196656, 3.0518977991517997};
    unit_assert("error, complex_cos(z1) != {2.0327230070196656, 3.0518977991517997}", complex_close(complex_cos(z1), r1));
}

static char *test_complex_tan(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t r1 = {-0.0338128260798967, 1.0147936161466335};
    unit_assert("error, complex_tan(z1) != {-0.0338128260798967, 1.0147936161466335}", complex_close(complex_tan(z1), r1));
}

static char *test_complex_sinh(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t r1 = {0.4890562590412937, 1.4031192506220405};
    unit_assert("error, complex_sinh(z1) != {0.4890562590412937, 1.4031192506220405}", complex_close(complex_sinh(z1), r1));
}

static char *test_complex_cosh(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t r1 = {-0.64214812471552, -1.0686074213827783};
    unit_assert("error, complex_cosh(z1) != {-0.64214812471552, -1.0686074213827783}", complex_close(complex_cosh(z1), r1));
}

static char *test_complex_tanh(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t r1 = {-1.16673625724092, -0.24345820118572534};
    unit_assert("error, complex_tanh(z1) != {-1.16673625724092, -0.24345820118572534}", complex_close(complex_tanh(z1), r1));
}

static char *test_complex_asin(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t r1 = {-0.4270785863924761, 1.5285709194809982};
    unit_assert("error, complex_asin(z1) != {-0.4270785863924761, 1.5285709194809982}", complex_close(complex_asin(z1), r1));
}

static char *test_complex_acos(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t r1 = {1.9978749131873728, -1.5285709194809982};
    unit_assert("error, complex_acos(z1) != {1.9978749131873728, -1.5285709194809982}", complex_close(complex_acos(z1), r1));
}

static char *test_complex_atan(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t r1 = {-1.3389725222944935, 0.40235947810852507};
    unit_assert("error, complex_atan(z1) != {-1.3389725222944935, 0.40235947810852507}", complex_close(complex_atan(z1), r1));
}

static char *test_complex_asinh(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t r1 = {-1.4693517443681852, 1.063440023577752};
    unit_assert("error, complex_asinh(z1) != {-1.4693517443681852, 1.063440023577752}", complex_close(complex_asinh(z1), r1));
}

static char *test_complex_acosh(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t r1 = {1.5285709194809982, 1.9978749131873728};
    unit_assert("error, complex_acosh(z1) != {1.5285709194809982, 1.9978749131873728}", complex_close(complex_acosh(z1), r1));
}

static char *test_complex_atanh(void) {
    complex_t z1 = {-1.0, 2.0};
    complex_t r1 = {-0.17328679513998632, 1.1780972450961724};
    unit_assert("error, complex_atanh(z1) != {-0.17328679513998632, 1.1780972450961724}", complex_close(complex_atanh(z1), r1));
}
