CC=tcc
CFLAGS=-g -Wall -Wextra -Wpedantic -I.

bin/00_fft1d.exe: obj/00_fft1d.o obj/fft.o
	$(CC) -o $@ $^

obj/00_fft1d.o: examples/00_fft1d.c
	$(CC) $(CFLAGS) -o $@ -c $<

obj/fft.o: fft.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	del obj/*.o
	del bin/*.exe

.PHONY: clean
