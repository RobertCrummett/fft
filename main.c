#include <stdio.h>
#include <stdlib.h>

#include "fft.h"

#define SIZE 12

int main(void) {
#ifdef REAL
	double *data = malloc(SIZE * sizeof *data);

	for (int i = 0; i < SIZE; i++)
		data[i] = (double)i;

	printf("time domain:\n");
	for (int i = 0; i < SIZE; i++)
		printf("%d: %lf %lf\n", i, data[i]);

	if (rfft1d(data, SIZE, FFT_FORWARD))
		return 1;

	cdouble *cdata = (cdouble *)data;

	printf("frequency domain:\n");
	for (int i = 0; i < SIZE / 2; i++)
		printf("%d: %lf %lf\n", i, cdata[i].real, cdata[i].imag);

	if (rfft1d(cdata, SIZE, FFT_INVERSE))
		return 1;

	printf("time domain:\n");
	for (int i = 0; i < SIZE; i++)
		printf("%d: %lf %lf\n", i, data[i]);
#else
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
#endif // REAL

	free(data);

	return 0;
}
