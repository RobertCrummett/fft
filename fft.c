#include "fft.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062
#endif // M_PI

enum fft_options {
	FFT_BITWISE_REVERSAL = 1 << 0,
	FFT_FORWARD_TWIDDLES = 1 << 1,
	FFT_INVERSE_TWIDDLES = 1 << 2,
	FFT_BUTTERFLY        = 1 << 3,
	FFT_NORMALIZE        = 1 << 4,
	FFT_CHIRPZ           = 1 << 5,
	FFT_ZERO_AUX         = 1 << 6,
	FFT_FORWARD_MIXING   = 1 << 7,
	FFT_INVERSE_MIXING   = 1 << 8,
	FFT_FORWARD_REAL     = 1 << 9,
	FFT_INVERSE_REAL     = 1 << 10,
};

/* ========================================================================== */
/*                             Complex Routines                               */
/* ========================================================================== */

static inline cdouble caddz(cdouble a, cdouble b)
{
	return COMPLEX(a.real + b.real, a.imag + b.imag);
}

static inline cdouble csubz(cdouble a, cdouble b)
{
	return COMPLEX(a.real - b.real, a.imag - b.imag);
}

static inline cdouble cmulz(cdouble a, cdouble b)
{
	const double ar = a.real;
	const double ai = a.imag;
	const double br = b.real;
	const double bi = b.imag;
	return COMPLEX(ar*br - ai*bi, ar*bi + ai*br);
}

static inline cdouble cdivz(cdouble a, cdouble b)
{
	const double ar = a.real;
	const double ai = a.imag;
	const double br = b.real;
	const double bi = b.imag;
	const double b2 = br * br + bi * bi;
	return COMPLEX((ar * br + ai * bi) / b2, (ai * br - ar * bi) / b2);
}

static inline cdouble cexpz(cdouble z)
{
	const double a = exp(z.real);
	const double x = cos(z.imag);
	const double y = sin(z.imag);
	return COMPLEX(a*x, a*y);
}

static inline cdouble conjz(cdouble z)
{
	return COMPLEX(z.real, -z.imag);
}

/* ========================================================================== */
/*                           Fast Fourier Transforms                          */
/* ========================================================================== */

void fftradix(cdouble *data, size_t size, cdouble *aux, size_t stride, int plan)
{
	if (plan & FFT_BITWISE_REVERSAL) {
		size_t target = 0;
		for (size_t position = 0; position < size; position++) {
			if (target > position) {
				cdouble temp = data[position * stride];
				data[position * stride] = data[target * stride];
				data[target * stride] = temp;
			}

			size_t mask = size >> 1;
			while (target & mask) {
				target &= ~mask;
				mask >>= 1;
			}
			target |= mask;
		}
	}

	const size_t halfsize = size >> 1; 

	if (plan & FFT_FORWARD_TWIDDLES) {
		for (size_t i = 0; i < halfsize; i++)
			aux[i] = cexpz(COMPLEX(0.0, -M_PI * i / halfsize));
	}

	if (plan & FFT_INVERSE_TWIDDLES) {
		for (size_t i = 0; i < halfsize; i++)
			aux[i] = cexpz(COMPLEX(0.0, M_PI * i / halfsize));
	}

	if (plan & FFT_BUTTERFLY) {
		for (size_t step = 1; step < size; step <<= 1) {
			const size_t jump = step << 1;
			cdouble twiddle = COMPLEX(1.0, 0.0);
			for (size_t group = 0; group < step; group++) {
				for (size_t pair = group; pair < size; pair += jump) {
					const size_t match = pair + step;
					const cdouble product = cmulz(twiddle, data[match * stride]);
					data[match * stride] = csubz(data[pair * stride], product);
					data[pair * stride] = caddz(data[pair * stride], product);
				}

				if (group + 1 == step) continue;

				twiddle = aux[(group + 1) * halfsize / step];
			}
		}
	}

	if (plan & FFT_NORMALIZE) {
		for (size_t i = 0; i < size; i++) {
			data[i * stride].real /= size; 
			data[i * stride].imag /= size;
		}
	}
}

void fft(cdouble *data, size_t size, cdouble *aux, size_t aux_size, size_t stride, int plan) 
{
	size_t larger_size = (2 * (aux_size - size)) / 5;

	if (plan & FFT_ZERO_AUX) {
		unsigned char *dst = (unsigned char *)(aux + size);
		for (size_t i = 0; i < 2 * larger_size * sizeof *aux; i++) {
			*dst = (unsigned char)0;
			dst++;
		}
	}

	if (plan & FFT_FORWARD_TWIDDLES) {
		for (size_t i = 0; i < size; i++)
			aux[i] = cexpz(COMPLEX(0.0, -M_PI * i * i / size));
	}

	if (plan & FFT_INVERSE_TWIDDLES) {
		for (size_t i = 0; i < size; i++)
			aux[i] = cexpz(COMPLEX(0.0, M_PI * i * i / size));
	}


	if (plan & FFT_CHIRPZ) {
		for (size_t i = 0; i < size; i++)
			aux[size + i] = cmulz(aux[i], data[i * stride]);

		aux[size + larger_size] = aux[0];
		for (size_t i = 1; i < size; i++) {
			aux[size + larger_size + i]     = conjz(aux[i]);
			aux[size + 2 * larger_size - i] = conjz(aux[i]);
		}

		fftradix(aux + size, larger_size, aux + size + 2 * larger_size, 1,
				FFT_BITWISE_REVERSAL |
				FFT_FORWARD_TWIDDLES |
				FFT_BUTTERFLY);

		fftradix(aux + size + larger_size, larger_size, aux + size + 2 * larger_size, 1,
				FFT_BITWISE_REVERSAL |
				FFT_BUTTERFLY);

		for (size_t i = 0; i < larger_size; i++)
			aux[size + i] = cmulz(aux[size + i], aux[size + larger_size + i]);

		fftradix(aux + size, larger_size, aux + size + 2 * larger_size, 1, 
				FFT_BITWISE_REVERSAL |
				FFT_INVERSE_TWIDDLES |
				FFT_BUTTERFLY |
				FFT_NORMALIZE);

		for (size_t i = 0; i < size; i++)
			data[i * stride] = cmulz(aux[size + i], aux[i]);
	}

	if (plan & FFT_NORMALIZE) {
		for (size_t i = 0; i < size; i++) {
			data[i * stride].real /= size; 
			data[i * stride].imag /= size; 
		}
	}
}

int fft1d(cdouble *data, size_t size, int forward)
{
	size_t larger_size = 1;
	while (larger_size >> 1 < size)
		larger_size <<= 1;

	size_t aux_size = size + 2 * larger_size + larger_size / 2;

	cdouble *aux = calloc(aux_size, sizeof *aux);
	if (!aux) {
		fprintf(stderr, "Failed to allocate auxillary space\n");
		return 1;
	}

	int plan = forward ?
		FFT_FORWARD_TWIDDLES | FFT_CHIRPZ : 
		FFT_INVERSE_TWIDDLES | FFT_CHIRPZ | FFT_NORMALIZE;

	fft(data, size, aux, aux_size, 1, plan);

	free(aux);

	return 0;
}

void rfft(cdouble *data, size_t size, cdouble *aux, size_t aux_size, size_t stride, int plan)
{
	if (plan & FFT_FORWARD_REAL)
		fft(data, size, aux, aux_size, stride,
				FFT_FORWARD_TWIDDLES |
				FFT_CHIRPZ);

	if (plan & FFT_INVERSE_REAL)
		fft(data, size, aux, aux_size, stride,
				FFT_INVERSE_TWIDDLES |
				FFT_CHIRPZ |
				FFT_NORMALIZE);

	if (plan & FFT_FORWARD_MIXING) {
		for (size_t i = 1; i < size; i++)
			aux[i] = cexpz(COMPLEX(0.0, -M_PI * i / size));

		data[0] = COMPLEX(data[0].real + data[0].imag, data[0].real - data[0].imag);

		for (size_t i = 1; i < size / 2 + 1; i++) {
			const size_t j = size - i;

			const cdouble Ci = data[i * stride];
			const cdouble Cj = data[j * stride];
			const cdouble Wi = aux[i];

			const cdouble Zx = cmulz(caddz(Ci, conjz(Cj)), COMPLEX(0.5, 0.0));
			const cdouble Zy = cmulz(csubz(Ci, conjz(Cj)), COMPLEX(0.0, -0.5));

			if (i == j)
				data[i * stride] = caddz(Zx, cmulz(Wi, Zy));
			else {
				const cdouble WiZy = cmulz(Wi, Zy);
				data[i * stride] = caddz(Zx, WiZy);
				data[j * stride] = conjz(csubz(Zx, WiZy));
			}
		}
	}
}

int rfft1d(double *data, size_t size, int forward)
{
	if (size & 1) {
		fprintf(stderr, "The real fft is not implemented on odd sized data\n");
		return 1;
	}

	cdouble *cdata = (cdouble *)data;
	const size_t half_size = size / 2;

	size_t half_larger_size = 1;
	while (half_larger_size >> 1 < half_size)
		half_larger_size <<= 1;

	size_t aux_size = half_size + 2 * half_larger_size + half_larger_size / 2;

	cdouble *aux = calloc(aux_size, sizeof *aux);
	if (!aux) {
		fprintf(stderr, "Failed to allocate auxillary space\n");
		return 1;
	}

	int plan = forward ? 
		FFT_FORWARD_REAL | FFT_FORWARD_MIXING :
		FFT_INVERSE_REAL | FFT_INVERSE_MIXING;

	rfft(cdata, half_size, aux, aux_size, 1, plan);

	free(aux);

	return 0;
}
