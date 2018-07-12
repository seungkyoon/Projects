#include <stdio.h>
#include <stdlib.h>

#include "deck.h"

typedef struct node {
    Card val;
    struct node * next;
} Node; //linked list to connect cards

struct deck {
	size_t length;
	Node * head;
	Node * tail;
}; //deck with length and reference to head (first) and tail (last) cards

Deck *deckCreate(void) {
	//malloc deck and temporary head
	Deck *newDeck = malloc(sizeof(Deck));
	newDeck->length = 52;
	newDeck->head = malloc(sizeof(Node));
	Node *current = newDeck->head;

	//for each suit and rank, set the next node to a new malloc and appropriate rank/suit
	for (int suit_i = 0; suit_i < 4; suit_i++) {
		for (int rank_i = 0; rank_i < 13; rank_i++) {
			current->next = malloc(sizeof(Node));
			current->next->val.rank = RANKS[rank_i];
			current->next->val.suit = SUITS[suit_i];
			current = current->next;
			current->next = NULL; //set current's next node to NULL (until overwritten)
		}
	}

	//remove temporary head and assign tail to last card (KS)
	Node * next_node = newDeck->head->next;
	free(newDeck->head);
	newDeck->head = next_node;
	newDeck->tail = current;

	//return deck
	return newDeck;
}

void deckDestroy(Deck *d) { //destroy deck by looping and freeing temp node until NULL; then free deck
	Node * temp;
    while (d->head != NULL) {
    	temp = d->head->next;
    	free(d->head);
    	d->head = temp;
    }
	free(d);
}

int deckNotEmpty(const Deck *d) { //if the length is 0, the deck is empty
	if (d->length==0) {
		return 0;
	}
	return 1;
}

Card deckGetCard(Deck *d) {
	//if deck is not empty, remove and free the head, assigning the next card to head
	Card temp;
	if (deckNotEmpty(d)) {
		temp = d->head->val;
		d->length--;
		Node * next_node = d->head->next;
		free(d->head);
		d->head = next_node;
		if (d->length==0) { //if length is now 0, set tail pointer to NULL
			d->tail = NULL;
		}
		return temp;
	}

	//if deck is empty, behavior undefined (return AC)
	temp.rank = RANKS[0];
	temp.suit = SUITS[0];
	return temp;
}

void deckPutCard(Deck *d, Card c) {
	//if deck is empty, add to length and set new tail to newly put card (also point tail's next to NULL)
	if (deckNotEmpty(d)) {
		d->length++;
		d->tail->next = malloc(sizeof(Node));
		d->tail->next->val = c;
		d->tail->next->next = NULL;
		d->tail = d->tail->next;
	} else { //else set head and tail to new card node
		d->length++;
		d->head = malloc(sizeof(Node));
		d->head->val = c;
		d->head->next = NULL;
		d->tail = d->head;
	}
}

void deckSplit(Deck *d, int n, Deck **d1, Deck **d2) {
	//malloc two decks
	*d1 = malloc(sizeof(Deck));
	*d2 = malloc(sizeof(Deck));

	//set *d1's length to n or d->length, whichever is smaller, and set *d2's length to the length of remainder of cards
	(*d1)->length = n;
	if (n > d->length) {
		(*d1)->length = d->length;
	}
	(*d2)->length = d->length - (*d1)->length;

	//if *d1's length is 0, set *d1's head and tail to NULL and *d2's to d's, freeing d
	if ((*d1)->length==0) {
		(*d1)->head = NULL;
		(*d1)->tail = NULL;
		(*d2)->head = d->head;
		(*d2)->tail = d->tail;
		free(d);
		return;
	}  else if((*d2)->length==0) { //if *d2's length is 0, set *d2's head and tail to NULL and *d1's to d's, freeing d
		(*d2)->head = NULL;
		(*d2)->tail = NULL;
		(*d1)->head = d->head;
		(*d1)->tail = d->tail;
		free(d);
		return;
	}

	//loop to find the tail of *d1
	Node * end = d->head;
	for (int i = 1; i<(*d1)->length; i++) {
		end = end->next;
	}

	//set *d1's and *d2's heads and tails along with the tails' nexts
	(*d2)->head = end->next;
	(*d2)->tail = d->tail;
	(*d2)->tail->next = NULL;
	(*d1)->head = d->head;
	(*d1)->tail = end;
	(*d1)->tail->next = NULL;

	//free d and terminate
	free(d);
	return;
}

Deck *deckShuffle(Deck *d1, Deck *d2) {
	//malloc deck and set length to sum of deck lengths
	Deck *d = malloc(sizeof(Deck));
	d->length = d1->length + d2->length;

	//if the length of deck is 0, free d1 and return d2 (which is an empty deck)
	if (d->length == 0) {
		free(d2);
		return d1;
	}

	//set head to a new node
	d->head = malloc(sizeof(Node));
	Node *current = d->head;
	Node *temp;

	//loop through d1 and d2 and build new deck while freeing and reassigning d1's and d2's heads until either head is null
	char parity = 0;
	while (d1->head!=NULL && d2->head!=NULL) {
		current->next = malloc(sizeof(Node));
		if (parity) { //every odd time, free d2's head and set new head to next node
			current->next->val = d2->head->val;
			temp = d2->head->next;
			free(d2->head);
			d2->head = temp;
		} else { //every even time, free d1's head and set new head to next node
			current->next->val = d1->head->val;
			temp = d1->head->next;
			free(d1->head);
			d1->head = temp;
		}
		current = current->next;

		parity = (parity + 1) % 2;
	}

	//if both heads are null, simply point current's next to NULL and set tail to current
	if (d1->head==NULL && d2->head==NULL) {
		current->next = NULL;
		d->tail = current;
	} else if (d1->head==NULL) { //if d1's head is null, point current's next to d2->head and tail to d2->tail
		current->next = d2->head;
		d->tail = d2->tail;
	} else if (d2->head==NULL) { //if d2's head is null, point current's next to d1->head and tail to d1->tail
		current->next = d1->head;
		d->tail = d1->tail;
	}

	//free d1 and d2 and free temporary head (reassign new head)
	free(d1);
	free(d2);
	temp = d->head->next;
	free(d->head);
	d->head = temp;

	//return deck
	return d;
}

void deckPrint(const Deck *d, FILE *f) {
	//if length is not 0, iterate through all nodes and print ranks and suits of cards to file
	if (d->length>0) {
		Node * current = d->head;
		fprintf(f, "%c%c", current->val.rank, current->val.suit);
		for (int i = 1; i<d->length; i++) {
			current = current->next;
			fprintf(f, " %c%c", current->val.rank, current->val.suit);
		}
	}
}
