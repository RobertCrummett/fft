#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef M_PI
#define RNC_PI M_PI
#else
#define RNC_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062
#endif

typedef struct cdouble {
	double real;
	double imag;
} cdouble;

#define RNC_COMPLEX(r,i) ((cdouble){(r),(i)})
#define RNC_IMAGINARY RNC_COMPLEX(0.0,1.0)

enum rnc_fftplan {
	RNC_FFT_BITWISE_REVERSAL = 1 << 0,
	RNC_FFT_FORWARD_TWIDDLES = 1 << 1,
	RNC_FFT_INVERSE_TWIDDLES = 1 << 2,
	RNC_FFT_BUTTERFLY        = 1 << 3,
	RNC_FFT_NORMALIZE        = 1 << 4,
	RNC_FFT_CHIRPZ           = 1 << 5,
	RNC_FFT_ZERO_AUX         = 1 << 6,
};

#define RNC_FFT_FORWARD 1
#define RNC_FFT_INVERSE 0

static inline cdouble rnc_caddz(cdouble a, cdouble b) {
	return RNC_COMPLEX(a.real + b.real, a.imag + b.imag);
}

static inline cdouble rnc_csubz(cdouble a, cdouble b) {
	return RNC_COMPLEX(a.real - b.real, a.imag - b.imag);
}

static inline cdouble rnc_cmulz(cdouble a, cdouble b) {
	const double ar = a.real;
	const double ai = a.imag;
	const double br = b.real;
	const double bi = b.imag;
	return RNC_COMPLEX(ar*br - ai*bi, ar*bi + ai*br);
}

static inline cdouble rnc_cdivz(cdouble a, cdouble b) {
	const double ar = a.real;
	const double ai = a.imag;
	const double br = b.real;
	const double bi = b.imag;
	const double b2 = br * br + bi * bi;
	return RNC_COMPLEX((ar * br + ai * bi) / b2, 
			(ai * br - ar * bi) / b2);
}

static inline cdouble rnc_cexpz(cdouble z) {
	const double a = exp(z.real);
	const double x = cos(z.imag);
	const double y = sin(z.imag);
	return RNC_COMPLEX(a*x, a*y);
}

static inline cdouble rnc_conjz(cdouble z) {
	return RNC_COMPLEX(z.real, -z.imag);
}

void rnc_fftradix(cdouble *data, size_t size, cdouble *aux, size_t stride, int plan) {
	if (plan & RNC_FFT_BITWISE_REVERSAL) {
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

	if (plan & RNC_FFT_FORWARD_TWIDDLES) {
		for (size_t i = 0; i < halfsize; i++)
			aux[i] = rnc_cexpz(RNC_COMPLEX(0.0, -RNC_PI * i / halfsize));
	}

	if (plan & RNC_FFT_INVERSE_TWIDDLES) {
		for (size_t i = 0; i < halfsize; i++)
			aux[i] = rnc_cexpz(RNC_COMPLEX(0.0, RNC_PI * i / halfsize));
	}

	if (plan & RNC_FFT_BUTTERFLY) {
		for (size_t step = 1; step < size; step <<= 1) {
			size_t jump = step << 1;
			cdouble twiddle = RNC_COMPLEX(1.0, 0.0);
			for (size_t group = 0; group < step; group++) {
				for (size_t pair = group; pair < size; pair += jump) {
					size_t match = pair + step;
					cdouble product = rnc_cmulz(twiddle, data[match * stride]);
					data[match * stride] = rnc_csubz(data[pair * stride], product);
					data[pair * stride] = rnc_caddz(data[pair * stride], product);
				}

				if (group + 1 == step) continue;

				twiddle = aux[(group + 1) * halfsize / step];
			}
		}
	}

	if (plan & RNC_FFT_NORMALIZE) {
		for (size_t i = 0; i < size; i++) {
			data[i * stride].real /= size; 
			data[i * stride].imag /= size;
		}
	}
}

void rnc_fft(cdouble *data, size_t size, cdouble *aux, size_t aux_size, size_t stride, int plan) {
	size_t larger_size = (2 * (aux_size - size)) / 5;

	if (plan & RNC_FFT_ZERO_AUX) {
		unsigned char *dst = (unsigned char *)(aux + size);
		for (size_t i = 0; i < 2 * larger_size * sizeof *aux; i++) {
			*dst = (unsigned char)0;
			dst++;
		}
	}

	if (plan & RNC_FFT_FORWARD_TWIDDLES) {
		for (size_t i = 0; i < size; i++)
			aux[i] = rnc_cexpz(RNC_COMPLEX(0.0, -RNC_PI * i * i / size));
	}

	if (plan & RNC_FFT_INVERSE_TWIDDLES) {
		for (size_t i = 0; i < size; i++)
			aux[i] = rnc_cexpz(RNC_COMPLEX(0.0,  RNC_PI * i * i / size));
	}


	if (plan & RNC_FFT_CHIRPZ) {
		for (size_t i = 0; i < size; i++)
			aux[size + i] = rnc_cmulz(aux[i], data[i * stride]);

		aux[size + larger_size] = aux[0];
		for (size_t i = 1; i < size; i++) {
			aux[size + larger_size + i]     = rnc_conjz(aux[i]);
			aux[size + 2 * larger_size - i] = rnc_conjz(aux[i]);
		}

		rnc_fftradix(aux + size, larger_size, aux + size + 2 * larger_size, 1,
				RNC_FFT_BITWISE_REVERSAL | RNC_FFT_FORWARD_TWIDDLES | RNC_FFT_BUTTERFLY);

		rnc_fftradix(aux + size + larger_size, larger_size, aux + size + 2 * larger_size, 1,
				RNC_FFT_BITWISE_REVERSAL | RNC_FFT_BUTTERFLY);

		for (size_t i = 0; i < larger_size; i++)
			aux[size + i] = rnc_cmulz(aux[size + i], aux[size + larger_size + i]);

		rnc_fftradix(aux + size, larger_size, aux + size + 2 * larger_size, 1, 
				RNC_FFT_BITWISE_REVERSAL | RNC_FFT_INVERSE_TWIDDLES | RNC_FFT_BUTTERFLY | RNC_FFT_NORMALIZE);

		for (size_t i = 0; i < size; i++)
			data[i * stride] = rnc_cmulz(aux[size + i], aux[i]);
	}

	if (plan & RNC_FFT_NORMALIZE) {
		for (size_t i = 0; i < size; i++) {
			data[i * stride].real /= size; 
			data[i * stride].imag /= size; 
		}
	}
}

int rnc_fft1d(cdouble *data, size_t size, int forward) {
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
		RNC_FFT_FORWARD_TWIDDLES | RNC_FFT_CHIRPZ : 
		RNC_FFT_INVERSE_TWIDDLES | RNC_FFT_CHIRPZ | RNC_FFT_NORMALIZE;

	rnc_fft(data, size, aux, aux_size, 1, plan);

	free(aux);

	return 0;
}

#define SIZE 8

int main(void) {
	cdouble *data = malloc(SIZE * sizeof *data);

	for (int i = 0; i < SIZE; i++)
		data[i] = RNC_COMPLEX((double)i, 0.0);

	printf("time domain:\n");
	for (int i = 0; i < SIZE; i++)
		printf("%d: %lf %lf\n", i, data[i].real, data[i].imag);

	if (rnc_fft1d(data, SIZE, RNC_FFT_FORWARD))
		return 1;

	printf("frequency domain:\n");
	for (int i = 0; i < SIZE; i++)
		printf("%d: %lf %lf\n", i, data[i].real, data[i].imag);

	if (rnc_fft1d(data, SIZE, RNC_FFT_INVERSE))
		return 1;

	printf("time domain:\n");
	for (int i = 0; i < SIZE; i++)
		printf("%d: %lf %lf\n", i, data[i].real, data[i].imag);

	return 0;
}
