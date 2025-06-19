#ifndef _RNC_FFT
#define _RNC_FFT

#include <stddef.h>

#define RNC_FFT_FORWARD 1
#define RNC_FFT_INVERSE 0

typedef struct cdouble {
	double real;
	double imag;
} cdouble;

#define RNC_COMPLEX(r,i) ((cdouble){(r),(i)})
#define RNC_IMAGINARY RNC_COMPLEX(0.0,1.0)

extern int rnc_fft1d(cdouble *data, size_t size, int forward);

#endif // _RNC_FFT
