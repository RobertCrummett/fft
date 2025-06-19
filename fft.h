#ifndef _FFT
#define _FFT

#include <stddef.h>

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
