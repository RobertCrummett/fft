#ifdef USE_FFT_PREFIX
#define FFT_PREFIX(name) (fft_##name)
#else
#define FFT_PREFIX(name) (name)
#endif // USE_FFT_PREFIX

#ifndef FFT_H
#define FFT_H

#include <complex.h>
#include <stddef.h>

#ifdef _MSC_VER
#define fft_complexd _Dcomplex
#else
#define fft_complexd complex double
#endif

/* Expected size of the auxillary space for the chirp Z transform and internal radix FFT calls */

// size_t larger_size = 1;
// while (larger_size >> 1 < size) larger_size <<= 1;
//
// fft_complexd* aux = calloc(size + 2 * larger_size + larger_size / 2, sizeof(*aux));
//

void FFT_PREFIX(fft)(fft_complexd *data, fft_complexd *aux, size_t size, size_t stride);
void FFT_PREFIX(ifft)(fft_complexd *data, fft_complexd *aux, size_t size, size_t stride);

#endif // FFT_H

#ifdef FFT_IMPLEMENTATION

#include <complex.h>
#include <math.h>
#include <stddef.h>
#include <string.h>

#ifdef _MSC_VER
#define fft_complexd _Dcomplex
#else
#define fft_complexd complex double
#endif

#ifndef FFT_PI
#define FFT_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062
#endif

static void fft_swap(fft_complexd* a, fft_complexd* b) {
	fft_complexd temp = *a;
	*a = *b;
	*b = temp;
}

static void fft_shuffle(fft_complexd* data, size_t size, size_t stride) {
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

static void fft_radixfft(fft_complexd *data, fft_complexd *aux, size_t size, size_t stride) {
	size_t i, step, jump, group, pair, match, halfsize = size / 2;
	fft_complexd twiddle, product;

	fft_shuffle(data, size, stride);

	for (i = 0; i < halfsize; i++)
		aux[i] = cexp(-I * FFT_PI * i / halfsize);

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

static void fft_radixifft(fft_complexd *data, fft_complexd *aux, size_t size, size_t stride) {
	size_t i, step, jump, group, pair, match, halfsize = size / 2;
	fft_complexd twiddle, product;

	fft_shuffle(data, size, stride);

	for (i = 0; i < halfsize; i++)
		aux[i] = cexp(I * FFT_PI * i / halfsize);

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

void FFT_PREFIX(fft)(fft_complexd *data, fft_complexd *aux, size_t size, size_t stride) {
	size_t larger_size = 1, i;
	while (larger_size >> 1 < size)
		larger_size <<= 1;

	for (i = 0; i < size; i++)
		aux[i] = cexp(-I * FFT_PI * i * i / size);

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

void FFT_PREFIX(ifft)(fft_complexd *data, fft_complexd *aux, size_t size, size_t stride) {
	size_t larger_size = 1, i;
	while (larger_size >> 1 < size)
		larger_size <<= 1;

	for (i = 0; i < size; i++)
		aux[i] = cexp(I * FFT_PI * i * i / size);

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

#endif // FFT_IMPLEMENTATION
