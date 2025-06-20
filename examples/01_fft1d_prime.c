// Example 01, FFT 1D Prime length array
// Run the FFT on a prime length array
// 
// [Robert] Nate Crummett
// robertcrummett@robertcrummett.com
// Licensed under 0BSD, see bottom

#include <stdio.h>
#include <stdlib.h>

#include "fft.h"

#define SIZE 11

int main(void) {
	// Create input array in the "time domain"
	cdouble *input = malloc(SIZE * sizeof *input);
	if (!input) return EXIT_FAILURE;

	for (int i = 0; i < SIZE; i++)
		input[i] = COMPLEX((double)i, 0.0);

	// Just as in the previous example, the input array is purely real.
	// But this time, we will use a prime length: 11

	// FFT implementations are often optimized for powers of two, but in
	// geophysics we are rarely fortunate enough to get data that is 
	// decimated (especially in the space domain). Therefore, it is imperitive
	// that the FFT work on arbirary lengthed arrays so that truncations (or
	// worse, extensions) are not applied to the observed data.

	printf("Original Input:\n");
	for (int i = 0; i < SIZE; i++)
		if (input[i].imag < 0.0)
			printf("[%02d] %.4lf - %.4lfi\n", i, input[i].real, -input[i].imag);
		else
			printf("[%02d] %.4lf + %.4lfi\n", i, input[i].real, input[i].imag);

	// Apply the one dimensional FFT
	if (fft1d(input, SIZE, FFT_FORWARD)) {
		free(input);
		return EXIT_FAILURE;
	}

	printf("\nFourier transform of input:\n");
	for (int i = 0; i < SIZE; i++)
		if (input[i].imag < 0.0)
			printf("[%02d] %.4lf - %.4lfi\n", i, input[i].real, -input[i].imag);
		else
			printf("[%02d] %.4lf + %.4lfi\n", i, input[i].real, input[i].imag);

	if (fft1d(input, SIZE, FFT_INVERSE)) {
		free(input);
		return EXIT_FAILURE;
	}

	printf("\nRecovered Input:\n");
	for (int i = 0; i < SIZE; i++)
		if (input[i].imag < 0.0)
			printf("[%02d] %.4lf - %.4lfi\n", i, input[i].real, -input[i].imag);
		else
			printf("[%02d] %.4lf + %.4lfi\n", i, input[i].real, input[i].imag);

	free(input);
	return EXIT_SUCCESS;
}

// Permission to use, copy, modify, and/or distribute this software for
// any purpose with or without fee is hereby granted.
// 
// THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL
// WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE
// FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY
// DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
// AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
// OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

