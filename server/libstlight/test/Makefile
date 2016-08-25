CC=gcc
CFLAGS=-Wall -I. -std=gnu99 -pedantic -march=native -Wno-pointer-arith
LDFLAGS=-lstlight

all: test

test: main.o
	$(CC) main.o -o test $(CFLAGS) $(LDFLAGS)

main.o: main.c
	$(CC) -c -fPIC main.c $(CFLAGS)

clean:
	-rm test
	-rm *.o