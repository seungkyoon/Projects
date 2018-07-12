#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "num.h"

struct num { //arbitrary length and char array of digits
	size_t length;
    char *digits;
};

//create new Num
Num * numCreate(const char *s) {
	Num *number = malloc(sizeof(Num));
	number->length = strlen(s);

	//if null string, return equivalent of 0
	if (number->length==0) {
		number->length = 1;
		number->digits = malloc(sizeof(char));
		number->digits[0] = 0;
		return number;
	}

	//allocate space for char array
	number->digits = malloc(sizeof(char) * number->length);

	//iterate through input string, putting numerical chars in number
	for (size_t i=0; i<number->length; i++) {
		number->digits[i] = s[number->length-i-1]-'0';

		if (number->digits[i]<0 || number->digits[i]>9) {
			numDestroy(number);
			return NULL;
		}
	}

	//remove any leading zeros
	for (size_t i=number->length-1; i>0; i--) {
		if (number->digits[i]==0) {
			number->length--;
		} else {
			break;
		}
	}

	//reallocate memory based on removed 0's
	char *newdigits = realloc(number->digits, sizeof(char) * number->length);
	number->digits = newdigits;

	return number;
}

//free digits and Num
void numDestroy(Num *n) {
	free(n->digits);
	free(n);
}

//returns digit if in range, 0 if not
int numGetDigit(const Num *n, int i) {
	if (i<n->length && i>-1) {
		return n->digits[i];
	}
	return 0;
}

//Add two numbers
Num * numAdd(const Num *x, const Num *y) {
	Num *number = malloc(sizeof(Num));

	//set length to longer length and allocate memory
	number->length = x->length + 1;
	if (y->length > x->length) {
		number->length = y->length + 1;
	}
	number->digits = malloc(sizeof(char) * number->length);

	//set carry=0 and iterate through all pairs of digits, adding and calculating the carry
	char carry = 0;
	for (size_t i = 0; i<number->length-1; i++) {
		number->digits[i] = (numGetDigit(x, i) + numGetDigit(y, i) + carry)%10;
		carry = (numGetDigit(x, i) + numGetDigit(y, i) + carry)/10;
	}

	//append the last carry if necessary (realloc if not)
	if (carry == 1) {
		number->digits[number->length-1] = 1;
	} else {
		number->length--;
		char *newdigits = realloc(number->digits, sizeof(char) * number->length);
		number->digits = newdigits;
	}

	return number;
}

//Multiply two numbers
Num * numMultiply(const Num *x, const Num *y) {
	//Allocate memory with length=sum of lengths (maximum possible length)
	Num *number = malloc(sizeof(Num));
	number->length = x->length+y->length;
	number->digits = calloc(number->length, sizeof(char));

	//set carry = 0 and do first set of multiplication (y * x0), keeping track of carry
	char carry = 0;
	for (size_t y_index = 0; y_index<y->length; y_index++) {
		number->digits[y_index+1] = (numGetDigit(y, y_index) * numGetDigit(x, 0) + numGetDigit(number, y_index)) / 10;
		number->digits[y_index] = (numGetDigit(y, y_index) * numGetDigit(x, 0) + numGetDigit(number, y_index)) % 10;
	}

	//do multiplication for rest of x and sum, keeping multiplication carry value (carry) and addition carry value (carry_two)
	char carry_two, temp, carry_temp;
	for (size_t x_index = 1; x_index<x->length; x_index++) {
		carry = 0;
		carry_two = 0;
		for (size_t y_index = 0; y_index<y->length; y_index++) {
			temp = numGetDigit(number, x_index+y_index);

			temp = (numGetDigit(y, y_index) * numGetDigit(x, x_index) + carry);
			carry = temp / 10; //carry of multiplication
			temp %= 10; //one's place of multiplication

			//calculate new carry_two then new value of digit based on previous and new carry_two
			carry_temp = carry_two;
			carry_two = (numGetDigit(number, x_index+y_index) + temp + carry_temp) / 10; 
			number->digits[x_index+y_index] = numGetDigit(number, x_index+y_index) + temp + carry_temp - 10*carry_two;
		}
		number->digits[x_index+y->length] = carry + carry_two; //add two carries for last digit
	}

	//remove leading zeros
	for (size_t i=number->length-1; i>0; i--) {
		if (number->digits[i]==0) {
			number->length--;
		} else {
			break;
		}
	}

	//realloc based on new length (after removing zeros)
	char *newdigits = realloc(number->digits, sizeof(char) * number->length);
	number->digits = newdigits;

	return number;
}

//print number to file
 void numPrint(const Num *n, FILE *f) {
	for (int i=n->length-1; i>-1; i--) {
		fprintf(f, "%d", n->digits[i]);
	}
}
