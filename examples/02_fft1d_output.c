// Example 02, FFT 1D Output
// Examination of the output of the one-dimensional FFT
// 
// [Robert] Nate Crummett
// robertcrummett@robertcrummett.com
// Licensed under 0BSD, see bottom

#include <stdio.h>
#include <stdlib.h>

#include "fft.h"

#define SIZE 10

int main(void) {
	// Create input array in the "time domain"
	cdouble *input = malloc(SIZE * sizeof *input);
	if (!input) return EXIT_FAILURE;

	for (int i = 0; i < SIZE; i++)
		input[i] = COMPLEX((double)i, 0.0);

	// Now to really understand the actual output of the FFT

	printf("Original Input:\n");
	for (int i = 0; i < SIZE; i++)
		if (input[i].imag < 0.0)
			printf("[%02d] %.4lf - %.4lfi\n", i, input[i].real, -input[i].imag);
		else
			printf("[%02d] %.4lf + %.4lfi\n", i, input[i].real, input[i].imag);

       	// I assume you know how C truncated unsigned integers.
	// Below, when I write "N/2", I mean the integer division of N by 2.
	//
	// N is the length of the input array. It can be even or odd

	// The DC frequency (0) is at index 0
	//
	// The Nyquist frequency (N/2) is at index N/2 for even length arrays 
	// For odd length arrays, the Nyquist frequency does not exist
	//
	// The positive frequencies are at indices [1, N/2-1]   for even length arrays
	//                                         [1, N/2]     for odd length arrays
	// The negative frequencies are at indices [N/2+1, N-1] for even length arrays
	//                                         [N/2+1, N-1] for odd length arrays

	if (fft1d(input, SIZE, FFT_FORWARD)) {
		free(input);
		return EXIT_FAILURE;
	}

	// Lets print the DC fequency.

	printf("\nDC frequency (index 0):\n");
	printf("[00] %.4lf + %.4lfi\n", input[0].real, input[0].imag);

	// Now lets print the Nyquist frequency, if it exists.

	if (SIZE & 1)
		printf("\nNyquist frequency does not exist for odd length arrays.\n");
	else {
		const int nyquist = SIZE / 2;
		printf("\nNyquist frequency (index %d):\n", nyquist);
		printf("[%02d] %.4lf + %.4lfi\n", nyquist, input[nyquist].real, input[nyquist].imag);
	}

	// Now for the positive and negative frequencies

	const int frequencies = SIZE / 2 + (SIZE & 1) - 1;

	printf("\nPositive and negative frequencies:\n");
	for (int i = 0; i < frequencies; i++) {
		const int positive = i + 1;
		const int negative = SIZE - i - 1;

		if (input[positive].imag < 0.0)
			printf("[%02d] %.4lf - %.4lfi ", positive, input[positive].real, -input[positive].imag);
		else
			printf("[%02d] %.4lf + %.4lfi ", positive, input[positive].real, input[positive].imag);

		if (input[negative].imag < 0.0)
			printf("[%02d] %.4lf - %.4lfi\n", negative, input[negative].real, -input[negative].imag);
		else
			printf("[%02d] %.4lf + %.4lfi\n", negative, input[negative].real, input[negative].imag);
	}

	// Notice that the positive and negative frequencies are conjugate symmetric when the input is purely real!
	// More on this later.

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


