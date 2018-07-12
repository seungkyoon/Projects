CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: testNum

testNum: num.o testNum.o
	$(CC) $(CFLAGS) -o $@ $^

test: testNum
	./testNum

clean:
	$(RM) testNum *.o

num.o: num.h num.c
testNum.o: testNum.c