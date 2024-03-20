#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// alphabet array will be modified to be the encrypted array.
char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// shuffle calls the fisher-yates algorithm and shuffles variable alphabet.
void shuffle(char str[26]){
	srand(time(NULL));
	int i = strlen(str);
	while(--i > 0){
		int randNum = rand() % i;
		char fc = str[i];
		char sc = str[randNum];
		str[randNum] = fc;
		str[i] = sc;
	}
}

// encryptPuzzle takes a string and modifies it using the alphabet variable. It calculates the distance from 'A' and gets the index from alphabet to replace the letter. Mallocs and returns a new string.
char* encryptPuzzle(char* input){
        int len = strlen(input);
        char* encrypted = malloc(len+1);
        for(int i = 0; i < len; i++){
                if(isalpha(input[i]) != 0){
                        char character = toupper(input[i]);
                        int loc = character - 'A';
			encrypted[i] = alphabet[loc];
                }
                else
                        encrypted[i] = input[i];
	}
      encrypted[len] = '\0';
      return encrypted;
}

// resetAlphabet reverts alphabet to its original state.
void resetAlphabet(){
    for (int i = 0; i < 26; i++)
        alphabet[i] = 'A' + i;
}

// reserUserAlphabet resets itself back to all null values.
void resetUserAlphabet(char* input){
for (int i = 0; i < 26; i++)
        input[i] = '\0';
}
