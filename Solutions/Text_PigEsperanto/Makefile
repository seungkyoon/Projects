CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: encode

encode: encode.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) encode *.o

encode.o: encode.c