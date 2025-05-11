#ifndef COMPLEX_H
#define COMPLEX_H

typedef struct complex_t {
    double real;
    double imag;
} complex_t;

extern complex_t complex_add(complex_t a, complex_t b);
extern complex_t complex_sub(complex_t a, complex_t b);
extern complex_t complex_mul(complex_t a, complex_t b);
extern complex_t complex_div(complex_t a, complex_t b);

extern double complex_real(complex_t z);
extern double complex_imag(complex_t z);
extern double complex_abs(complex_t z);
extern double complex_arg(complex_t z);

extern complex_t complex_conj(complex_t z);
extern complex_t complex_proj(complex_t z);
extern complex_t complex_exp(complex_t z);
extern complex_t complex_log(complex_t z);
extern complex_t complex_pow(complex_t base, complex_t exponent);
extern complex_t complex_sqrt(complex_t z);

extern complex_t complex_sin(complex_t z);
extern complex_t complex_cos(complex_t z);
extern complex_t complex_tan(complex_t z);

extern complex_t complex_sinh(complex_t z);
extern complex_t complex_cosh(complex_t z);
extern complex_t complex_tanh(complex_t z);

extern complex_t complex_asin(complex_t z);
extern complex_t complex_acos(complex_t z);
extern complex_t complex_atan(complex_t z);

extern complex_t complex_asinh(complex_t z);
extern complex_t complex_acosh(complex_t z);
extern complex_t complex_atanh(complex_t z);

#endif // COMPLEX_H

#ifdef COMPLEX_IMPLEMENTATION

#include <math.h>

#ifndef COMPLEX_PI
#define COMPLEX_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062
#endif

complex_t complex_add(complex_t a, complex_t b) {
    return (complex_t){complex_real(a)+complex_real(b), complex_imag(a)+complex_imag(b)};
}

complex_t complex_sub(complex_t a, complex_t b) {
    return (complex_t){complex_real(a)-complex_real(b), complex_imag(a)-complex_imag(b)};
}

complex_t complex_mul(complex_t a, complex_t b) {
    return (complex_t){complex_real(a)*complex_real(b)-complex_imag(a)*complex_imag(b),
                       complex_real(a)*complex_imag(b)+complex_imag(a)*complex_real(b)};
}

complex_t complex_div(complex_t a, complex_t b) {
    double b2 = complex_real(b)*complex_real(b) + complex_imag(b)*complex_imag(b);
    return (complex_t){(complex_real(a)*complex_real(b)+complex_imag(a)*complex_imag(b))/b2,
                       (complex_imag(a)*complex_real(b)-complex_real(a)*complex_imag(b))/b2};
}

double complex_real(complex_t z) {
    return z.real;
}

double complex_imag(complex_t z) {
    return z.imag;
}

double complex_abs(complex_t z) {
    // TODO: remove sqrt
    return sqrt(complex_real(z)*complex_real(z) + complex_imag(z)*complex_imag(z));
}

double complex_arg(complex_t z) {
    // TODO: remove atan2
    return atan2(complex_imag(z), complex_real(z));
}

complex_t complex_conj(complex_t z) {
    return (complex_t){complex_real(z), -complex_imag(z)};
}

complex_t complex_proj(complex_t z) {
    // TODO: Remove INFINITY, copysign, isinf
    if (isinf(complex_real(z)) || isinf(complex_imag(z)))
        return (complex_t){INFINITY, copysign(0.0, complex_imag(z))};
    else
        return z;
}

complex_t complex_exp(complex_t z) {
    // TODO: Remove exp, sin, cos
    double a = exp(complex_real(z));
    double x = cos(complex_imag(z));
    double y = sin(complex_imag(z));
    return (complex_t){a*x, a*y};
}

complex_t complex_log(complex_t z) {
    // TODO: Remove log
    return (complex_t){log(complex_abs(z)), complex_arg(z)};
}

complex_t complex_pow(complex_t base, complex_t exponent) {
    return complex_exp(complex_mul(complex_log(base), exponent));
}

complex_t complex_sqrt(complex_t z) {
    const complex_t half = {0.5, 0.0};
    return complex_exp(complex_mul(complex_log(z), half));
}

complex_t complex_sin(complex_t z) {
    double x = complex_real(z);
    double y = complex_imag(z);
    // TODO: Remove cos, sin, cosh, sinh
    return (complex_t){sin(x)*cosh(y), cos(x)*sinh(y)};
}

complex_t complex_cos(complex_t z) {
    double x = complex_real(z);
    double y = complex_imag(z);
    // TODO: Remove cos, sin, cosh, sinh
    return (complex_t){cos(x)*cosh(y), -sin(x)*sinh(y)};
}

complex_t complex_tan(complex_t z) {
    return complex_div(complex_sin(z), complex_cos(z));
}

complex_t complex_sinh(complex_t z) {
    const complex_t ipos = {0.0, 1.0};
    const complex_t ineg = {0.0,-1.0};
    return complex_mul(ineg, complex_sin(complex_mul(ipos, z)));
}

complex_t complex_cosh(complex_t z) {
    const complex_t ipos = {0.0, 1.0};
    return complex_cos(complex_mul(ipos, z));
}

complex_t complex_tanh(complex_t z) {
    const complex_t ipos = {0.0, 1.0};
    const complex_t ineg = {0.0,-1.0};
    return complex_mul(ineg, complex_tan(complex_mul(ipos, z)));
}

complex_t complex_asin(complex_t z) {
    const complex_t ipos = {0.0, 1.0};
    const complex_t ineg = {0.0,-1.0};
    return complex_mul(ineg, complex_asinh(complex_mul(ipos, z)));
}

complex_t complex_acos(complex_t z) {
    const complex_t ipos = {0.0, 1.0};
    const complex_t one = {1.0, 0.0};
    const complex_t half_pi = {COMPLEX_PI / 2.0, 0.0};
    
    complex_t a = complex_sqrt(complex_sub(one, complex_mul(z, z)));
    complex_t b = complex_log(complex_add(complex_mul(ipos, z), a));

    return complex_add(half_pi, complex_mul(ipos, b));
}

complex_t complex_atan(complex_t z) {
    const complex_t ipos = {0.0, 1.0};
    const complex_t ineg = {0.0,-1.0};
    return complex_mul(ineg, complex_atanh(complex_mul(ipos, z)));
}

complex_t complex_asinh(complex_t z) {
    const complex_t one = {1.0, 0.0};

    complex_t a = complex_sub(one, complex_mul(z, z));
    complex_t b = complex_add(z, complex_sqrt(a));

    return complex_log(b);
}

complex_t complex_acosh(complex_t z) {
    const complex_t one = {1.0, 0.0};
    
    complex_t a = complex_sqrt(complex_add(z, one));
    complex_t b = complex_sqrt(complex_sub(z, one));
    complex_t c = complex_add(z, complex_mul(a, b));

    return complex_log(c);
}

complex_t complex_atanh(complex_t z) {
    const complex_t one = {1.0, 0.0};
    const complex_t two = {2.0, 0.0};

    complex_t a = complex_log(complex_add(one, z));
    complex_t b = complex_log(complex_sub(z, one));
    complex_t c = complex_sub(a, b);
    
    return complex_div(c, two);
}

#endif // COMPLEX_IMPLEMENTATION
