#include <stdio.h>
#include <stdlib.h>

int
main (int argc, char **argv) {
	//check argument formatting
	if (argc!=4 || atoi(argv[1])<1) {
		return 1;
	}

	//declare variables and initialize the arguments n, a, b
	int n, a, b, index;
	n = atoi(argv[1]);
	a = atoi(argv[2]);
	b = atoi(argv[3]);
	int *seq = (int *) malloc(sizeof(int) * n);
	int ch;

	//run until end of file
	while ((ch = getchar()) != EOF) {
		seq[0] = ch;

		//put chars into heap ('\0' if no more chars)
		for (index = 1; index < n; index++) {
			seq[index] = getchar();
			if (seq[index]==EOF) {
				seq[index] = ('\0');
			}
		}

		//outputs chars based on encryption
		for (index = 0; index < n; index++) {
			putchar(seq[((((long)a * index + b) % n) + n) % n]);
		}
	}

	free(seq);
	return 0;
}
