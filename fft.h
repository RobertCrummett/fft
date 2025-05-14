#ifndef FFT_H
#define FFT_H

#include <stddef.h>
#include "complex.h"

#ifdef FFT_STATIC
#define FFT_PUBLICDEC static
#define FFT_PUBLICDEF static
#else
#define FFT_PUBLICDEC extern
#define FFT_PUBLICDEF
#endif // FFT_STATIC

/* Expected size of the auxillary space for the chirp Z transform and internal radix FFT calls */

// size_t larger_size = 1;
// while (larger_size >> 1 < size) larger_size <<= 1;
//
// complex_t* aux = calloc(size + 2 * larger_size + larger_size / 2, sizeof(*aux));
//
FFT_PUBLICDEC void fft_fft(complex_t *data, complex_t *aux, size_t size, size_t stride);
FFT_PUBLICDEC void fft_ifft(complex_t *data, complex_t *aux, size_t size, size_t stride);

#endif // FFT_H

#ifdef FFT_IMPLEMENTATION

#include <stddef.h>
#include "complex.h"

#ifndef FFT_PI
#define FFT_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062
#endif // FFT_PI

#ifndef FFT_I
#define FFT_I ((complex_t){0.0, 1.0})
#endif // FFT_I

static void *fft_memset(void *s, int c, size_t n) {
    unsigned char *dst = s;
    while (n > 0) {
        *dst = (unsigned char)c;
        dst++;
        n--;
    }
    return s;
}

static void fft_swap(complex_t* a, complex_t* b) {
	complex_t temp = *a;
	*a = *b;
	*b = temp;
}

static void fft_shuffle(complex_t* data, size_t size, size_t stride) {
	size_t position, mask, target = 0;
	for (position = 0; position < size; position++) {
		if (target > position)
			fft_swap(&data[position * stride], &data[target * stride]);

		mask = size >> 1;

		while (target & mask) {
			target &= ~mask;
			mask >>= 1;
		}
		target |= mask;
	}
}

static void fft_radixfft(complex_t *data, complex_t *aux, size_t size, size_t stride) {
	size_t i, step, jump, group, pair, match, halfsize = size / 2;
	complex_t twiddle, product;

	fft_shuffle(data, size, stride);

	for (i = 0; i < halfsize; i++)
		aux[i] = complex_exp(complex_mul(FFT_I, (complex_t){-FFT_PI * i / halfsize, 0.0}));

	for (step = 1; step < size; step <<= 1) {
		jump = step << 1;
		twiddle = (complex_t){1.0, 0.0};
		for (group = 0; group < step; group++) {
			for (pair = group; pair < size; pair += jump) {
				match = pair + step;
				product = complex_mul(twiddle, data[match * stride]);
				data[match * stride] = complex_sub(data[pair * stride], product);
				data[pair * stride] = complex_add(data[pair * stride], product);
			}

			if (group + 1 == step)
				continue;

			twiddle = aux[(group + 1) * halfsize / step];
		}
	}
}

static void fft_radixifft(complex_t *data, complex_t *aux, size_t size, size_t stride) {
	size_t i, step, jump, group, pair, match, halfsize = size / 2;
	complex_t twiddle, product;

	fft_shuffle(data, size, stride);

	for (i = 0; i < halfsize; i++)
		aux[i] = complex_exp(complex_mul(FFT_I, (complex_t){FFT_PI * i / halfsize, 0.0}));


	for (step = 1; step < size; step <<= 1) {
		jump = step << 1;
		twiddle = (complex_t){1.0, 0.0};
		for (group = 0; group < step; group++) {
			for (pair = group; pair < size; pair += jump) {
				match = pair + step;
				product = complex_mul(twiddle, data[match * stride]);
				data[match * stride] = complex_sub(data[pair * stride], product);
				data[pair * stride] = complex_add(data[pair * stride], product);
			}

			if (group + 1 == step)
				continue;

			twiddle = aux[(group + 1) * halfsize / step];
		}
	}
}

FFT_PUBLICDEF void fft_fft(complex_t *data, complex_t *aux, size_t size, size_t stride) {
	size_t larger_size = 1, i;
	while (larger_size >> 1 < size)
		larger_size <<= 1;

	for (i = 0; i < size; i++)
		aux[i] = complex_exp(complex_mul(FFT_I, (complex_t){-FFT_PI * i * i / size, 0.0}));

	for (i = 0; i < size; i++)
		aux[size + i] = complex_mul(aux[i], data[i * stride]);

	fft_memset(aux + size + larger_size, 0, larger_size * sizeof(*aux));
	aux[size + larger_size] = aux[0];
	for (i = 1; i < size; i++) {
		aux[size + larger_size + i]     = complex_conj(aux[i]);
		aux[size + 2 * larger_size - i] = complex_conj(aux[i]);
	}

	fft_radixfft(aux + size, aux + size + 2 * larger_size, larger_size, 1);
	fft_radixfft(aux + size + larger_size, aux + size + 2 * larger_size, larger_size, 1);

	for (i = 0; i < larger_size; i++)
		aux[size + i] = complex_mul(aux[size + i], aux[size + larger_size + i]);

	fft_radixifft(aux + size, aux + size + 2 * larger_size, larger_size, 1);

	for (i = 0; i < size; i++)
		data[i * stride] = complex_div(complex_mul(aux[size + i], aux[i]), (complex_t){larger_size, 0.0});
}

FFT_PUBLICDEF void fft_ifft(complex_t *data, complex_t *aux, size_t size, size_t stride) {
	size_t larger_size = 1, i;
	while (larger_size >> 1 < size)
		larger_size <<= 1;

	for (i = 0; i < size; i++)
		aux[i] = complex_exp(complex_mul(FFT_I, (complex_t){FFT_PI * i * i / size, 0.0}));

	for (i = 0; i < size; i++)
		aux[size + i] = complex_mul(aux[i], data[i * stride]);

	fft_memset(aux + size + larger_size, 0, larger_size * sizeof(*aux));
	aux[size + larger_size] = aux[0];
	for (i = 1; i < size; i++) {
		aux[size + larger_size + i]     = complex_conj(aux[i]);
		aux[size + 2 * larger_size - i] = complex_conj(aux[i]);
	}

	fft_radixfft(aux + size, aux + size + 2 * larger_size,larger_size, 1);
	fft_radixfft(aux + size + larger_size, aux + size + 2 * larger_size, larger_size, 1);

	for (i = 0; i < larger_size; i++)
		aux[size + i] = complex_mul(aux[size + i], aux[size + larger_size + i]);

	fft_radixifft(aux + size, aux + size + 2 * larger_size, larger_size, 1);

	for (i = 0; i < size; i++)
		data[i * stride] = complex_div(complex_mul(aux[size + i], aux[i]), (complex_t){larger_size, 0.0});
}

#endif // FFT_IMPLEMENTATION
