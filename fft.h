#ifndef FFT_H
#define FFT_H

#include <complex.h>
#include <stddef.h>

/* Expected size of the auxillary space for the chirp Z transform and internal radix FFT calls */

// size_t larger_size = 1;
// while (larger_size >> 1 < size) larger_size <<= 1;
//
// complex double* aux = calloc(size + 2 * larger_size + larger_size / 2, sizeof(*aux));
//

extern void fft_fft(complex double *data, complex double *aux, size_t size, size_t stride);
extern void fft_ifft(complex double *data, complex double *aux, size_t size, size_t stride);

#endif
