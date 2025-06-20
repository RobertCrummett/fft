Fast Fourier Transform
----------------------

While I could have taken a highly optimized FFT from the interweb and used it, I decided to roll my own.
The reason I prefer to ''recreate the wheel'' is that I wanted to _learn_. Deal with it.

See the `examples` directory for documented tutorials on usage.

Dependencies
------------

No third party dependencies.

The file `fft.h` depends on
- `stdlib.h` for memory allocation in high level functions `fft1d` and `rfft1d`. Lower level functions in `fft.c` rely on auxillary memeory, and make no allocations of their own.
- `stdio.h` for error messages. Only relevant in the high level functions `fft1d` and `rfft1d`.
- `math.h` for `cos`, `sin`, and `exp` in the complex exponentation function `cexpz`

I did not include `complex.h` because it becomes painful to make things cross platform.
Instead, I rolled my own necessary subset of `complex.h` for the FFT and threw them into `fft.c`.
So if you see `cdouble` and not `complex_t`, just know it is defined like

```c
typedef struct cdouble {
  double real;
  double imag;
} cdouble;
```

Building
--------

As you could probably expect, to build an object file

```console
gcc -o fft.o -c fft.c
```

This should work with other compilers as well. I used `tcc`, the [Tiny C Compiler](https://www.bellard.org/tcc/), in the `Makefile` provided.

Implementation Details
----------------------

The real FFT routines only work for even length arrays. I have not yet figured out how to get them to work on odd length arrays.

The real FFT `rfft1d` takes an input array `data` of type `double`.
Upon successfully calling the function, reinterpret the array as one of complex numbers:

```c
cdouble *cdata = (cdouble *)data;
```

Note the length of `cdata` is half the length of `data`!

- The DC component is the real part of the 0'th index: `= cdata[0].real`
- The Nyquist component is the imaginary part of the 0'th index: `= cdata[0].imag`
- The positive frequency components are listed in increasing order from index 1 to the end of the array
- The negative frequency components are not explicitly stored; however, they can be recovered in constant time by taking the corresponing positive frequency and applying the complex conjugate (implemented here in `conjz`) to it
