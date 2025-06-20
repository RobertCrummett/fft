// Example 00, FFT 1D
// No manual auxillary memory allocation
// 
// [Robert] Nate Crummett
// robertcrummett@robertcrummett.com
// Licensed under the Zero-Clause BSD License, see below for details.

#include <stdio.h>
#include <stdlib.h>

#include "fft.h"

#define SIZE 8

int main(void) {
	// Create input array in the "time domain"

	cdouble *input = malloc(SIZE * sizeof *input);
	if (!input) return 1;

	for (int i = 0; i < SIZE; i++)
		input[i] = COMPLEX((double)i, 0.0);

	// The input array is purely real.

	// Therefore, the positive and negative freqencies of the fourier transform
	// should be conjugate symmetric. Additionally, the DC and Nyquist frequencies
	// should be purely real. These are ways to cognitively examine ``correctness''.

	printf("Original Input:\n");
	for (int i = 0; i < SIZE; i++)
		if (input[i].imag < 0.0)
			printf("[%d] %.4lf - %.4lfi\n", i, input[i].real, -input[i].imag);
		else
			printf("[%d] %.4lf + %.4lfi\n", i, input[i].real, input[i].imag);

	// Apply the one dimensional FFT directly to this array to push it into the
	// "frequency domain"

	// We check for a non zero exit code after applying `fft1d` because 
	// internal allocation of auxillary memory occurs.

	if (fft1d(input, SIZE, FFT_FORWARD)) {
		free(input);
		return 1;
	}

	printf("\nFourier transform of input:\n");
	for (int i = 0; i < SIZE; i++)
		if (input[i].imag < 0.0)
			printf("[%d] %.4lf - %.4lfi\n", i, input[i].real, -input[i].imag);
		else
			printf("[%d] %.4lf + %.4lfi\n", i, input[i].real, input[i].imag);

	// Now we can apply the inverse FFT to return to the "time domain"

	if (fft1d(input, SIZE, FFT_INVERSE)) {
		free(input);
		return 1;
	}

	printf("\nRecovered Input:\n");
	for (int i = 0; i < SIZE; i++)
		if (input[i].imag < 0.0)
			printf("[%d] %.4lf - %.4lfi\n", i, input[i].real, -input[i].imag);
		else
			printf("[%d] %.4lf + %.4lfi\n", i, input[i].real, input[i].imag);

	free(input);
	return 0;
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
