#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "num.c"

#define TEST_ARRAY_LENGTH (10)

#define BASE (10)

/* print a Num to stdout followed by newline */
static void
numPut(const Num *n)
{
    numPrint(n, stdout);
    putchar('\n');
}

int
main(int argc, char **argv)
{
    Num *n;
    Num *n2;
    Num *x;
    Num *y;
    Num *a[TEST_ARRAY_LENGTH];
    char s[2];
    FILE *f;
    char *big;
    int bigSize;
    int squaringSteps;

    if(argc < 2) {
        fprintf(stderr, "Usage: %s test-code [test arguments]\n", argv[0]);
        return 1;
    }

    switch(argv[1][0]) {

        case 'p':
            /* test parsing and printing */
            n = numCreate(argv[2]);
            if(n == 0) {
                puts("numCreate returns 0");
            } else {
                numPut(n);
                numDestroy(n);
            }
            break;

        case 'a':
            /* test creating many numbers from same input string location */
            s[1] = '\0';

            for(int i = 0; i < TEST_ARRAY_LENGTH; i++) {
                s[0] = i + '0';
                a[i] = numCreate(s);
            }

            for(int i = 0; i < TEST_ARRAY_LENGTH; i++) {
                numPut(a[i]);
            }

            for(int i = 0; i < TEST_ARRAY_LENGTH; i++) {
                numDestroy(a[i]);
            }

            break;

        case 'g':
            /* test getting digits from a big number */
            bigSize = atoi(argv[2]);

            big = malloc(bigSize+1);

            for(int i = 0; i < bigSize; i++) {
                big[i] = '0' + (i % BASE);
            }

            big[bigSize] = '\0';

            n = numCreate(big);

            free(big);

            /* watch for leading zero */
            for(int i = 0; i < bigSize-1; i++) {
                assert(numGetDigit(n, bigSize - i - 1) == i % BASE);
            }

            /* check larger values are all 0 */
            for(int i = bigSize - 1; i < 2*bigSize; i++) {
                assert(numGetDigit(n, i) == 0);
            }

            /* spam some negative values too */
            for(int i = -bigSize; i < 0; i++) {
                assert(numGetDigit(n, i ) == 0);
            }

            numDestroy(n);

            break;

        case 's':
            /* test addition */
            x = numCreate(argv[2]);
            y = numCreate(argv[3]);

            n = numAdd(x,y);

            numDestroy(x);
            numDestroy(y);

            numPut(n);

            numDestroy(n);
            
            break;

        case 'm':
            /* test multiplication */
            x = numCreate(argv[2]);
            y = numCreate(argv[3]);

            n = numMultiply(x,y);

            numDestroy(x);
            numDestroy(y);

            numPut(n);

            numDestroy(n);
            
            break;

        case 'q':
            /* repeated squaring */
            n = numCreate(argv[2]);
            squaringSteps = atoi(argv[3]);

            for(int i = 0; i < squaringSteps; i++) {
                n2 = numMultiply(n, n);
                numDestroy(n);
                n = n2;
            }

            numPut(n);
            numDestroy(n);

            break;

        case 'f':
            /* test output to a file other than stdout */
            f = fopen(argv[2], "w");
            assert(f);

            n = numCreate(argv[3]);
            assert(n);

            numPrint(n, f);

            numDestroy(n);
            fclose(f);

            break;

        default:
            fprintf(stderr, "%s: unrecognized test code %c\n", argv[0], argv[1][0]);
            return 1;

            break;
    }

    return 0;
}

// int
// main (int argc, char **argv) {
//  Num *number = numCreate("12341521");
//  fprintf(stdout, "%d\n", number->length);
//  fprintf(stdout, "%d", numGetDigit(number, -1));
//  fprintf(stdout, "%d", numGetDigit(number, 0));
//  fprintf(stdout, "%d", numGetDigit(number, 1));
//  fprintf(stdout, "%d", numGetDigit(number, 2));
//  fprintf(stdout, "%d", numGetDigit(number, 3));
//  fprintf(stdout, "%d", numGetDigit(number, 4));
//  fprintf(stdout, "%d\n", numGetDigit(number, 5));
//  numPrint(number, stdout);
//  puts("\n");

//  Num *zero = numCreate("");
//  fprintf(stdout, "%d\n", zero->length);
//  fprintf(stdout, "%d", numGetDigit(zero, -1));
//  fprintf(stdout, "%d", numGetDigit(zero, 0));
//  fprintf(stdout, "%d\n", numGetDigit(zero, 1));
//  numPrint(zero, stdout);
//  puts("\n");

//  Num *attempt = numCreate("-123123");
//  if (attempt==NULL){
//      puts("thank god\n");
//  }

//  Num *sum = numAdd(number, number);
//  fprintf(stdout, "%d\n", sum->length);
//  fprintf(stdout, "%d", numGetDigit(sum, -1));
//  fprintf(stdout, "%d", numGetDigit(sum, 0));
//  fprintf(stdout, "%d", numGetDigit(sum, 1));
//  fprintf(stdout, "%d", numGetDigit(sum, 2));
//  fprintf(stdout, "%d", numGetDigit(sum, 3));
//  fprintf(stdout, "%d", numGetDigit(sum, 4));
//  fprintf(stdout, "%d\n", numGetDigit(sum, 5));
//  numPrint(sum, stdout);
//  puts("\n");

//  Num *one = numCreate("98765");

//  Num *prod = numMultiply(number, one);
//  fprintf(stdout, "%d\n", prod->length);
//  fprintf(stdout, "%d", numGetDigit(prod, -1));
//  fprintf(stdout, "%d", numGetDigit(prod, 0));
//  fprintf(stdout, "%d", numGetDigit(prod, 1));
//  fprintf(stdout, "%d", numGetDigit(prod, 2));
//  fprintf(stdout, "%d", numGetDigit(prod, 3));
//  fprintf(stdout, "%d", numGetDigit(prod, 4));
//  fprintf(stdout, "%d\n", numGetDigit(prod, 5));
//  numPrint(prod, stdout);
//  puts("\n");

//  numDestroy(number);
//  numDestroy(zero);
//  numDestroy(sum);
//  numDestroy(one);
//  numDestroy(prod);


//  return 1;
// }