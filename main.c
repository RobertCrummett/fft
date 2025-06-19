#include <stdio.h>
#include <stdlib.h>

#include "fft.h"

#define SIZE 8

int main(void) {
	cdouble *data = malloc(SIZE * sizeof *data);

	for (int i = 0; i < SIZE; i++)
		data[i] = COMPLEX((double)i, 0.0);

	printf("time domain:\n");
	for (int i = 0; i < SIZE; i++)
		printf("%d: %lf %lf\n", i, data[i].real, data[i].imag);

	if (fft1d(data, SIZE, FFT_FORWARD))
		return 1;

	printf("frequency domain:\n");
	for (int i = 0; i < SIZE; i++)
		printf("%d: %lf %lf\n", i, data[i].real, data[i].imag);

	if (fft1d(data, SIZE, FFT_INVERSE))
		return 1;

	printf("time domain:\n");
	for (int i = 0; i < SIZE; i++)
		printf("%d: %lf %lf\n", i, data[i].real, data[i].imag);

	return 0;
}
