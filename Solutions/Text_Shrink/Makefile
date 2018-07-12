CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: shrink

shrink: shrink.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) shrink *.o

shrink.o: shrink.c