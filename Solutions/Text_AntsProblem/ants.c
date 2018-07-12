#include <stdio.h>
#include <stdlib.h>

typedef struct ant {
	int character;
    int x;
    int y;
    int z;
    struct ant * next;
} Ant; //linked list chaining for hashtable values

#define tableSize (8011)
int hashFunc(int num) {
	return ((num % tableSize) + tableSize) % tableSize;
}

void writeAnt(Ant **table, int pos[3], int character) { //write new character to position
	int hash = hashFunc(pos[0]+pos[1]+pos[2]); //hash position
	if (table[hash]==NULL) { //if no existing values with hash key, write ant as first value in table
		table[hash] = malloc(sizeof(Ant));
		table[hash]->character = character;
		table[hash]->x = pos[0];
		table[hash]->y = pos[1];
		table[hash]->z = pos[2];
		table[hash]->next = NULL;
	} else {  //else, look for place to input new value
		Ant *current = table[hash];
		while (current->next!=NULL && !(current->x==pos[0] && current->y==pos[1] && current->z==pos[2])) {
			current=current->next;
		}
		if (current->x==pos[0] && current->y==pos[1] && current->z==pos[2]) { //overwrite if x, y, z exist
			current->character = character;
		} else { //append new ant write to end of existing ant writes
			current->next = malloc(sizeof(Ant));
			current->next->character = character;
			current->next->x = pos[0];
			current->next->y = pos[1];
			current->next->z = pos[2];
			current->next->next = NULL;
		}
	}
}

void readChar(Ant **table, int pos[3]) { //read character at position
	int hash = hashFunc(pos[0]+pos[1]+pos[2]); //hash position
	if (table[hash]==NULL) { //if no values with hash key, return ' '
		putchar(' ');
	} else {
		Ant *current = table[hash];
		while (current!=NULL && !(current->x==pos[0] && current->y==pos[1] && current->z==pos[2])) {
			current=current->next;
		}
		if (current==NULL) { //if nothing at x, y, z position, return ' '
			putchar(' ');
		} else { //else, return character at position
			putchar(current->character);
		}
	}
}

void destroyTable(Ant **table) {
	Ant *temp;
	for (int i=0; i<tableSize; i++) { //for each hash
		while (table[i]!=NULL) { //free chain
			temp=table[i]->next;
			free(table[i]);
			table[i] = temp;
		}
	}
	free(table); //free table
}

int
main (int argc, char **argv) {
	Ant **table; //table is array of ant pointers
	table = malloc(tableSize*sizeof(Ant *));
	for (int i=0; i<tableSize; i++) { //initialize table to null pointers
		table[i] = NULL;
	}

	int pos[256][3] = {0}; //initialize all positions of ants in 2D array to 0

	int ch; //new character
	int curr = getchar(); //current ant
	while ((ch = getchar()) != EOF) {
		switch(ch) {
			case 'h' : //move west
				pos[curr][0]--;
				break;
			case 'j' : //move south
				pos[curr][1]--;
				break;
			case 'k' : //move north
				pos[curr][1]++;
				break;
			case 'l' : //move east
				pos[curr][0]++;
				break;
			case '<' : //move up
				pos[curr][2]++;
				break;
			case '>' : //move down
				pos[curr][2]--;
				break;
			case '*' : //move to 2x current position
				pos[curr][0]*= 2;
				pos[curr][1]*= 2;
				pos[curr][2]*= 2;
				break;
			case '.' : //write character to position
				writeAnt(table, pos[curr], curr);
				break;
			case '?' : //read character at position
				readChar(table, pos[curr]);
				break;
			case '\n' : //newline (new ant)
				curr = getchar();
				break;
		}
	}	

	destroyTable(table); //destroy table
	return 0;
}
