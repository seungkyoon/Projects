CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: treeSort

treeSort: treeSort.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) treeSort *.o

treeSort.o: treeSort.c