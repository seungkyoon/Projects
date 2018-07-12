#include <stdio.h>
#include <string.h>
#include <ctype.h>

int
main (int argc, char **argv) {
	int length;
	int ch, new;

	// loops until end of file
	while ((ch = getchar()) != EOF) {
		//finds first alpha character
		if (isalpha(ch)) {
			length = 1;

			//set second alpha character to case of first character
			if (isalpha(new = getchar())) {
				if (isupper(ch)) {
					new = toupper(new);
				} else {
					new = tolower(new);
				}
			}

			//put the rest of the alpha characters
			while (isalpha(new)) {
				putchar(new);
				length++;
				new = getchar();
			}

			//after finding first non-alpha, set ending depending on length
			if (length < 4) {
				if (length == 1) {
					putchar(ch);
				} else {
					putchar(tolower(ch));
				}
				putchar('a');
				putchar('n');
			} else {
				putchar(tolower(ch));
				putchar('o');
			}

			//put last non-alpha character if not end of file
			if (new != EOF) {
				putchar(new);
			}

		//put non-alpha chars directly
		} else {
			putchar(ch);
		}
	}

	return (0);
}
