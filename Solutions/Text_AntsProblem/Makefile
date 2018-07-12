CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: ants

ants: ants.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) ants *.o

ants.o: ants.c