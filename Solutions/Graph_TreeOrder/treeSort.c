#include <stdio.h>
#include <stdlib.h>

typedef struct array { //Array representation of tree
    size_t size;			//size of tree
    size_t numChildren;		//number of children
    struct array **subtrees;//has array of array pointers
} Array;

int isPowerOfTwo(size_t x) { //check if power of two
    return (x & (x - 1)) == 0;
}

int compareTrees(const void * p, const void * q) { //compare two trees' size
    return ((*((Array **) q))->size - (*((Array **) p))->size);
}

void print(Array *tree) { //print open bracket, children's subtrees, then closed bracket
	putchar('[');
	for (int i=0; i<tree->numChildren; i++) {
		print(tree->subtrees[i]);
	}
	putchar(']');
}

void destroy(Array *tree) { //destroy all subtrees, then free subtree array (if it exists) and tree
	for (int i=0; i<tree->numChildren; i++) {
		destroy(tree->subtrees[i]);
	}
	if (tree->numChildren>0) {
		free(tree->subtrees);
	}
	free(tree);
}

Array *create() {
	int ch;
	Array *tree = malloc(sizeof(Array)); //create new array with size 1 (itself) and no children
	tree->size = 1;
	tree->numChildren = 0;
	
	if ((ch = getchar()) == '[') { //if it has a child, add to #children, malloc subtree, and add subtree size to tree size
		tree->numChildren++;
		tree->subtrees = malloc(sizeof(Array *));
		if ((tree->subtrees[0] = create()) == 0) { //free and return 0 if there is an improper array
			free(tree->subtrees);
			free(tree);
			return 0;
		}
		tree->size += (tree->subtrees[0])->size;
		while ((ch = getchar()) == '[') { //if there are children, increment number, create new subtree, and add subtree size to tree size
			if (isPowerOfTwo(tree->numChildren)) { //realloc two times the space if the tree size is a power of two
				tree->subtrees = realloc(tree->subtrees, 2*tree->numChildren*sizeof(Array *));
			}

			tree->numChildren++;
			if ((tree->subtrees[(tree->numChildren)-1] = create()) == 0) { //free and return 0 if there is an improper array
				tree->numChildren--;
				destroy(tree);
				return 0;
			}
			tree->size += tree->subtrees[(tree->numChildren)-1]->size;
		}
		qsort((void *) tree->subtrees, tree->numChildren, sizeof(Array *), compareTrees); //sort tree
	}

	if (ch != ']') { //destroy and return 0 if non-bracket
		destroy(tree);
		return 0;
	}

	return tree;
}

int
main (int argc, char **argv) {
	if (getchar() != '[') { //if it does not start with a [, break
		return 1;
	}
	
	Array *tree = create(); //create tree
	if (tree == 0) { //return 1 if create returns 0
		return 1;
	}
	print(tree);
	destroy(tree);
	return 0;
}
