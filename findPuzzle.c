#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

// quote sturcture holds block, author, and next.
struct quote{
	char* block;
	char* author;
	struct quote *next;
};

// quotes initialized to hold all the quotes from the quotes file.
struct quote* quotes;

// flptr initiated to read through the file.
FILE *fptr;

// quotesSize counts how many quotes are present in the file.
int quotesSize = 0;

// initQuote initiates quotes
void initQuote(struct quote** input){
	char* block = NULL;
	char* author = NULL;
	struct quote *next = NULL;
}

// copy safely copies bufer onto quote withour risk of segmentation fault.
// It simply copies each character onto quote without allocating any memory.
void copy(char quote[2000], char buffer[1000]){
	char c = buffer[0];
	int i = 0;
	while(c != '\0'){
		quote[i] = buffer[i];
		c = buffer[i];
		i++;
	}
}

// append safely appends buffer to quote without risk of segmentation fault.
// It simply copies characters from index 0 to strlen without making any
// allocations
void append(char quote[2000],  char line[100]){
	int len = strlen(quote);
	int n = 0;
	char c = line[n];
	while(c != '\0'){
	quote[len+n] = line[n];
	n++;
	c = line[n];
	}
	quote[len+n] = '\0';
}

// addItem creates a pointer and iterates it to the next NULL node. It then
// allocates memory for the structure, the quote & author inputs. It adds 1
// to the quotesSize.
void addItem(char quote[2000], char auth[500]){
	struct quote** iteratedNode = &quotes;
	while (*iteratedNode != NULL)
        	iteratedNode = &(*iteratedNode)->next;
	*iteratedNode = (struct quote*)malloc(sizeof(struct quote));
	(*iteratedNode) -> block = strdup(quote);
	(*iteratedNode) -> author = strdup(auth);
	(*iteratedNode) -> next = NULL;
	quotesSize++;
}

// populateFields iterates through file until it reaches the end. On the way
// it parses quotes, authors, and blank lines. It configures the lines it's
// parsing over to send off to addItem, where it then appends to the linkedlist.
void populateFields(struct quote** input){
	char buffer[2000];
	char quote[2000];
	char author[500];
	bool canAppend = false;
	fgets(buffer, sizeof(buffer), fptr);
	buffer[sizeof(buffer) - 1] = '\0';	
	while((buffer[0] != '-' || buffer[1] != '-')){
		int len = strlen(buffer);
		buffer[len] = '\0';
		if(buffer[0] != '\r'){
			if(canAppend == false){
				copy(quote, buffer);
				canAppend = true;
			}
			else
				append(quote, buffer);
		}
		fgets(buffer, sizeof(buffer), fptr);
	}
	copy(author, buffer);
	fgets(buffer, sizeof(buffer), fptr);
        buffer[sizeof(buffer) - 1] = '\0';
	if(feof(fptr) == 0){
		while(buffer[0] != '\r'){
			append(author, buffer);
			fgets(buffer, sizeof(buffer), fptr);
		}
		addItem(quote, author);
	}
}

// loadPuzzle finds the quotes file and reads it. If fptr is null then it returns 1 and send an error message.
int loadPuzzles(){
	fptr = fopen("quotes.txt", "r");
	if(fptr == NULL){
		printf("Warning! Missing file \"quotes.txt!\"");
		return 1;
	}
	struct quote** current = &quotes;
	while(feof(fptr) == 0){
		populateFields(current);
		current = &(*current) -> next;	
	}
	fclose(fptr);
	return 0;
}

// printQuotes is used for debugging purposes.
void printQuotes(){
	struct quote* iteratedNode = quotes;
	int i = 0;
	while(iteratedNode != NULL){
		printf("Quote#%d: %s\nAuthor#%d: %s\n", i,(iteratedNode)->block, i,(iteratedNode)->author);
		iteratedNode = (iteratedNode)->next;	
		i++;
	}
}

// freeQuotes takes quote and iterates through each node. It frees each node while saving the next. It does this
// until iteratedNode is set to null.
void freeQuotes(){
	struct quote* iteratedNode = quotes;
	struct quote* iteratedNextNode;
	while(iteratedNode != NULL){
		iteratedNextNode = iteratedNode->next;
		free(iteratedNode->block);
		free(iteratedNode->author);
		free(iteratedNode);
		iteratedNode = iteratedNextNode;
	}
	quotes = NULL;
}

// getRandQuote takes a random number, iterates through the list until n == randNum, and then returns a string
// of the block attribute in the linkedList item.
char* getRandQuote(){
	srand(time(NULL));
	int randNum = rand() % quotesSize;
	int n = 0;
	struct quote* iteratedNode = quotes;
	while(n != randNum){
		iteratedNode = (iteratedNode)->next;
		n++;
	}
	size_t len = strlen(iteratedNode->block) + strlen(iteratedNode->author) + 1;
	char* fin = malloc(len);
	strcpy(fin, (iteratedNode)->block);
	strcat(fin, (iteratedNode)->author);
	return fin;
}

// initiateFile is a helper method which automatically initiates quotes & files without worrying about calling quotes within the main script.
// This is for organizational purposes.
int initiateFile(){
	initQuote(&quotes);
	if(loadPuzzles() == 1)
		return 1;
	return 0;
}
