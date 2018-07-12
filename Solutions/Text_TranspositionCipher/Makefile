CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: transpose

transpose: transpose.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) transpose *.o

transpose.o: tranpose.c