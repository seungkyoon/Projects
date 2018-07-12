CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: testDeck

testDeck: deck.o testDeck.o
	$(CC) $(CFLAGS) -o $@ $^

test: testDeck
	./testDeck

clean:
	$(RM) testDeck *.o

deck.o: deck.h deck.c
testDeck.o: testDeck.c