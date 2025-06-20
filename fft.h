// Fast Fourier Transform (FFT) public interface
//
// [Robert] Nate Crummett
// robertcrummett@robertcrummett.com

#ifndef _FFT
#define _FFT

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define FFT_FORWARD 1
#define FFT_INVERSE 0

typedef struct cdouble {
	double real;
	double imag;
} cdouble;

#define COMPLEX(r,i) ((cdouble){(r),(i)})
#define IMAGINARY COMPLEX(0.0,1.0)

extern int fft1d(cdouble *data, size_t size, int forward);
extern int rfft1d(double *data, size_t size, int forward);

#endif // _FFT

/* Permission to use, copy, modify, and/or distribute this software for       */
/* any purpose with or without fee is hereby granted.                         */
/*                                                                            */
/* THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL              */
/* WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES   */
/* OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE     */
/* FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY         */
/* DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN */
/* AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT    */
/* OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.          */
