#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **print;

//Linked list for modification and pulling
typedef struct node {
	char character;
    struct node * next;
} Node;

//Linked list for chaining (hash table)
typedef struct chain {
	char * word;
    struct chain * next;
} Chain;

#define tableSize (32531)
int hashFunc(int length, char *string) { //hash function adds each character and multiplies by 4 (modded by table size)
	int val = 1;
	for(int i=0; i<length; i++) {
		val += string[i];
		val *= 43;
	}
	return ((val % tableSize) + tableSize) % tableSize;
}

char *toString(int length, Node *head) { //converts linked list to string (malloced)
	char *string = malloc((length+1)*sizeof(char));
	Node *temp = head;
	for (int i=0; i < length; i++) {
		string[i] = temp->character;
		temp = temp->next;
	}
	string[length] = '\0';
	return string;
}

int checkTable(int length, char *string, Chain **table) { //check table for string (else, add to table)
	int hash = hashFunc(length, string);
	if (table[hash]!=NULL) { //if dictionary entry not empty
		Chain *current = table[hash];
		for (; current->next != NULL; current = current->next) {
			if (strcmp(string, current->word) == 0) { //if word in table
				return 1;
			}
		}
		if (strcmp(string, current->word) == 0) { //if last word in table
			return 1;
		}
		current->next = malloc(sizeof(Chain));
		current->next->word = string;
		current->next->next = NULL;
	} else { //if empty, add new entry
		table[hash] = malloc(sizeof(Chain));
		table[hash]->word = string;
		table[hash]->next = NULL;
	}
	return 0;
}

int diff(char a, char b) { //if difference in ascii values 5 or both vowels
	if (((a-b)>-6 && (a-b)<6) || (((a=='a')||(a=='e')||(a=='i')||(a=='o')||(a=='u')) && ((b=='a')||(b=='e')||(b=='i')||(b=='o')||(b=='u')))) {
		return 1;
	} else {
		return 0;
	}
}


int search(int length, Node *head, Chain **table) { //main search function takes length, linked list, and table of checked strings
	char *string = toString(length, head); //convert to string

	if (checkTable(length, string, table)) { //put into table; free string and return if it already exists in table
		free(string);
		return 0;
	}

	if (length==1) { //if length=1, put into print array and return 1 (solution found)
		print[0] = string;
		return 1;
	}

	Node *temp;
	char a;
	for (Node *current = head; current->next!=NULL; current = current->next) { //go through the whole linked list
		if (diff(current->character, current->next->character)) { //if adjacent characters match
			temp = current->next; //store temporary node and temporary character
			a = current->character;
			current->character = current->next->character; //set current character to next character
			current->next = current->next->next;
			if (search(length-1, head, table)) { //if solution found recursively, free temporary node, add to print array, and return 1 (solution found)
				free(temp);
				print[length-1] = string;
				return 1;
			} else { //reset linked list values and continue
				current->next = temp;
				current->character = a;
			}
		}
		if (current->next->next!=NULL && current->next->next->next!=NULL) { //if sandwiched characters match
			if (diff(current->character, current->next->next->next->character)) {
				temp = current->next->next->next; //store temporary node and temporary character
				a = current->character;
				current->character = current->next->next->next->character; //set current character to later character
				current->next->next->next = current->next->next->next->next;
				if (search(length-1, head, table)) { //if solution found recursively, free temporary node, add to print array, and return 1 (solution found)
					free(temp);
					print[length-1] = string;
					return 1;
				} else { //reset linked list values and continue
					current->next->next->next = temp;
					current->character = a;
				}
			}
		}
	}

	return 0; //if all failed, return 0
}

void destroyList(Node *head) { //destroy linked list (free until head is null)
	Node * temp;
    while (head != NULL) {
    	temp = head->next;
    	free(head);
    	head = temp;
    }
}

void destroyTable(Chain **table) { //destroy hash table for each hash then chain
	Chain * temp;
	for (int i=0; i<tableSize; i++) { //for each hash
		while (table[i]!=NULL) { //free chain
			temp=table[i]->next;
			free(table[i]->word);
			free(table[i]);
			table[i] = temp;
		}
	}
	free(table); //free table
}

int
main (int argc, char **argv) {
	Chain **table = malloc(tableSize*sizeof(Chain *)); //initialize hash table and pointers to null
	for (int i=0; i<tableSize; i++) {
		table[i] = NULL;
	}

	//build linked list from string argv[1]
	char *string = argv[1];
	int length = strlen(string);
	Node *head = malloc(sizeof(Node));
	head->character = string[0];
	Node *temp = head;
	for (int i=1; i<length; i++) {
		temp->next = malloc(sizeof(Node));
		temp->next->character = string[i];
		temp = temp->next;
	}
	temp->next = NULL;

	//search then print array if solution exists
	print = malloc(length * sizeof(char *));
	if (search(length, head, table)) {
		for (int i=length-1; i>=0; i--) {
			printf("%s\n", print[i]);
		}
	}

	//destroy linked list, hash table, and free print array
	destroyList(head);
	destroyTable(table);
	free(print);
	return 0;
}