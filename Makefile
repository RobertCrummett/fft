main.exe: main.o rnc_fft.o
	tcc -o $@ $^

main.o: main.c
	tcc -o $@ -c $<

rnc_fft.o: rnc_fft.c
	tcc -o $@ -c $<
