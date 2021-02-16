# MAKEFILE
# Řešení IJC-DU1, 20.3.2019 
# Autor: Michal Koval, FIT 	


CC= gcc
CFLAGS = -g -std=c11 -pedantic -Wall -Wextra -O2
CC= gcc
ARCH= -m64

all: primes primes-i steg-decode

clean:
	rm primes primes-i steg-decode *.o

run:
	make all
	ulimit -s 20000 && time ./primes
	ulimit -s 20000 && time ./primes-i

# MACRO PRIMES
primes: primes.o eratosthenes.o error.o
	$(CC) $(CFLAGS) $(ARCH) primes.o eratosthenes.o error.o -o primes -lm

primes.o: primes.c eratosthenes.h bit_array.h
	$(CC) $(CFLAGS) $(ARCH) -c primes.c -o primes.o -lm

eratosthenes.o: eratosthenes.c eratosthenes.h bit_array.h
	$(CC) $(CFLAGS) $(ARCH) -c eratosthenes.c -o eratosthenes.o -lm

# INLINE PRIMES
primes-i: primes-i.o error.o eratosthenes-i.o 
	$(CC) $(CFLAGS) $(ARCH) -DUSE_INLINE primes-i.o eratosthenes-i.o error.o -o primes-i -lm
	
primes-i.o: primes.c bit_array.h eratosthenes.h
	$(CC) $(CFLAGS) $(ARCH) -c -DUSE_INLINE primes.c -o primes-i.o -lm
	
eratosthenes-i.o: eratosthenes.c eratosthenes.h bit_array.h
	$(CC) $(CFLAGS) $(ARCH) -c -DUSE_INLINE eratosthenes.c -o eratosthenes-i.o -lm

# STEG-DECODE
steg-decode: ppm.o error.o steg-decode.o eratosthenes.o 
	$(CC) $(CFLAGS) $(ARCH) ppm.o error.o steg-decode.o eratosthenes.o -o steg-decode -lm
	
steg-decode.o: steg-decode.c eratosthenes.h bit_array.h error.h ppm.h error.c
	$(CC) $(CFLAGS) $(ARCH) -c steg-decode.c -o steg-decode.o -lm

error.o: error.c error.h
	$(CC) $(CFLAGS) $(ARCH) -c error.c -o error.o

ppm.o: ppm.c error.h ppm.h error.c
	$(CC) $(CFLAGS) $(ARCH) -c ppm.c -o ppm.o	
