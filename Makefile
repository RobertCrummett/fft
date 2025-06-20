CC=tcc
CFLAGS=-g -Wall -Wextra -Wpedantic

main.exe: main.o fft.o
	$(CC) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -o $@ -c $<

fft.o: fft.c
	$(CC) $(CFLAGS) -o $@ -c $<
