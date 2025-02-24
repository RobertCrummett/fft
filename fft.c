#include <complex.h>
#include <math.h>
#include <stddef.h>
#include <string.h>

#include "fft.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062
#endif

void fft_swap(complex double* a, complex double* b) {
	complex double temp = *a;
	*a = *b;
	*b = temp;
}

void fft_shuffle(complex double* data, size_t size, size_t stride) {
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

void fft_radixfft(complex double *data, complex double *aux, size_t size, size_t stride) {
	size_t i, step, jump, group, pair, match, halfsize = size / 2;
	complex double twiddle, product;

	fft_shuffle(data, size, stride);

	for (i = 0; i < halfsize; i++)
		aux[i] = cexp(-I * M_PI * i / halfsize);

	for (step = 1; step < size; step <<= 1) {
		jump = step << 1;
		twiddle = 1.0 + 0.0 * I;
		for (group = 0; group < step; group++) {
			for (pair = group; pair < size; pair += jump) {
				match = pair + step;
				product = twiddle * data[match * stride];
				data[match * stride] = data[pair * stride] - product;
				data[pair * stride] += product;
			}

			if (group + 1 == step)
				continue;

			twiddle = aux[(group + 1) * halfsize / step];
		}
	}
}

void fft_radixifft(complex double *data, complex double *aux, size_t size, size_t stride) {
	size_t i, step, jump, group, pair, match, halfsize = size / 2;
	complex double twiddle, product;

	fft_shuffle(data, size, stride);

	for (i = 0; i < halfsize; i++)
		aux[i] = cexp(I * M_PI * i / halfsize);

	for (step = 1; step < size; step <<= 1) {
		jump = step << 1;
		twiddle = 1.0 + 0.0 * I;
		for (group = 0; group < step; group++) {
			for (pair = group; pair < size; pair += jump) {
				match = pair + step;
				product = twiddle * data[match * stride];
				data[match * stride] = data[pair * stride] - product;
				data[pair * stride] += product;
			}

			if (group + 1 == step)
				continue;

			twiddle = aux[(group + 1) * halfsize / step];
		}
	}
}

void fft_fft(complex double *data, complex double *aux, size_t size, size_t stride) {
	size_t larger_size = 1, i;
	while (larger_size >> 1 < size)
		larger_size <<= 1;

	for (i = 0; i < size; i++)
		aux[i] = cexp(-I * M_PI * i * i / size);

	for (i = 0; i < size; i++)
		aux[size + i] = aux[i] * data[i * stride];

	memset(aux + size + larger_size, 0, larger_size * sizeof(*aux));
	aux[size + larger_size] = aux[0];
	for (i = 1; i < size; i++) {
		aux[size + larger_size + i]     = conj(aux[i]);
		aux[size + 2 * larger_size - i] = conj(aux[i]);
	}

	fft_radixfft(aux + size, aux + size + 2 * larger_size, larger_size, 1);
	fft_radixfft(aux + size + larger_size, aux + size + 2 * larger_size, larger_size, 1);

	for (i = 0; i < larger_size; i++)
		aux[size + i] *= aux[size + larger_size + i];

	fft_radixifft(aux + size, aux + size + 2 * larger_size, larger_size, 1);

	for (i = 0; i < size; i++)
		data[i * stride] = aux[size + i] * aux[i] / larger_size;
}

void fft_ifft(complex double *data, complex double *aux, size_t size, size_t stride) {
	size_t larger_size = 1, i;
	while (larger_size >> 1 < size)
		larger_size <<= 1;

	for (i = 0; i < size; i++)
		aux[i] = cexp(I * M_PI * i * i / size);

	for (i = 0; i < size; i++)
		aux[size + i] = aux[i] * data[i * stride];

	memset(aux + size + larger_size, 0, larger_size * sizeof(*aux));
	aux[size + larger_size] = aux[0];
	for (i = 1; i < size; i++) {
		aux[size + larger_size + i]     = conj(aux[i]);
		aux[size + 2 * larger_size - i] = conj(aux[i]);
	}

	fft_radixfft(aux + size, aux + size + 2 * larger_size,larger_size, 1);
	fft_radixfft(aux + size + larger_size, aux + size + 2 * larger_size, larger_size, 1);

	for (i = 0; i < larger_size; i++)
		aux[size + i] *= aux[size + larger_size + i];

	fft_radixifft(aux + size, aux + size + 2 * larger_size, larger_size, 1);

	for (i = 0; i < size; i++)
		data[i * stride] = aux[size + i] * aux[i] / larger_size;
}
