#include <stdlib.h>
#include <stdio.h>
#include "array.h"

//holds combine function, #leaves, sum of leaves, and left and right subtrees
struct array {
	int (*combine)(int, int);
	size_t leaves;
	int sum;
	Array *left;
	Array *right;
};

//creates array using combine function and size of array in the subtree
Array *arrayCreate(int (*combine)(int, int), size_t n) { //
	Array *head = malloc(sizeof(Array)); //create head with combine function and #leaves in array
	head->combine = combine;
	head->leaves = n;
	size_t half = n/2; //size of each subtree is half of total array size
	if (half) { //if size is not 1, recursively build tree and combine using combine function
		head->left = arrayCreate(combine, n - half);
		head->right = arrayCreate(combine, half);
		head->sum = combine(head->left->sum, head->right->sum);
	} else { //if size is 1, simply set sum equal to 0
		head->left = NULL;
		head->right = NULL;
		head->sum = 0;
	}
	return head; //return head
}

void arrayDestroy(Array *head) { //destroy head (recursively destroy subtrees then free head)
	if (head->left) {
		arrayDestroy(head->left);
	}
	if (head->right) {
		arrayDestroy(head->right);
	}
	free(head);
}

size_t arraySize(const Array * head) { //return number of leaves
	return head->leaves;
}

//return sum if leaf node, otherwise recursively search left or right subtrees
int arrayGet(const Array * head, size_t i) {
	if (head->left == NULL && head->right == NULL) { //leaf
		return head->sum;
	} else if (i >= 0 && i < head->left->leaves) { //left
		return arrayGet(head->left, i);
	} else if (i >= head->left->leaves && i < head->leaves) { //right
		return arrayGet(head->right, i - head->left->leaves);
	} else { //out of range
		return 0;
	}
}

//set sum to v if leaf node, otherwise recursively search left or right subtree appropriately and update sums along path
void arraySet(Array * head, size_t i, int v) {
	if (head->left == NULL && head->right == NULL) { //leaf
		head->sum = v;
	} else if (i >= 0 && i < head->left->leaves) { //left
		arraySet(head->left, i, v);
		head->sum = head->combine(head->left->sum, head->right->sum);
	} else if (i >= head->left->leaves && i < head->leaves) { //right
		arraySet(head->right, i - head->left->leaves, v);
		head->sum = head->combine(head->left->sum, head->right->sum);
	}
}

//return sum if leaf node, otherwise search left and right subtrees (combining sum of left subtree if in right subtree)
int arrayCombine(const Array * head, size_t k) {
	if (head->left == NULL && head->right == NULL) { //leaf
		return head->sum;
	} else if (k > 0 && k <= head->left->leaves) { //left
		return arrayCombine(head->left, k);
	} else { //right or else
		return head->combine(head->left->sum, arrayCombine(head->right, k - head->left->leaves));
	}
}

// int sum (int a, int b) {
// 	return a+b+1;
// }

// int
// main (int argc, char **argv) {
// 	Array *array = arrayCreate(sum, 0);
// 	arraySet(array, 0, 10);
// 	printf("%d", arrayGet(array, 0));
// 	arrayDestroy(array);
// 	return 0;
// }