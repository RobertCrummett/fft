CC=tcc
CFLAGS=-g -Wall -Wextra -Wpedantic -I.

all: 00_fft1d.exe 01_fft1d_prime.exe 02_fft1d_output.exe


02_fft1d_output.exe: 02_fft1d_output.o fft.o
	$(CC) -o $@ $^

01_fft1d_prime.exe: 01_fft1d_prime.o fft.o
	$(CC) -o $@ $^

00_fft1d.exe: 00_fft1d.o fft.o
	$(CC) -o $@ $^


02_fft1d_output.o: examples/02_fft1d_output.c
	$(CC) $(CFLAGS) -o $@ -c $<

01_fft1d_prime.o: examples/01_fft1d_prime.c
	$(CC) $(CFLAGS) -o $@ -c $<

00_fft1d.o: examples/00_fft1d.c
	$(CC) $(CFLAGS) -o $@ -c $<

fft.o: fft.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	del *.o
	del *.exe

.PHONY: clean all
