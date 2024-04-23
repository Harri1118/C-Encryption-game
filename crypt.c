#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "encrypt.c"
#include "findPuzzle.c"

// gameContents to keep track of the user's guess and the answer.
struct gameContents{
	char* puzzle;
	char* answer;
};

// gameContents struct initialized.
struct gameContents game;

// input for user to enter.
//char input[100];

// userAlphabet which will record the user input.
char userAlphabet[26];

// answerSize defines the character size of the answer.
int answerSize = 0;

char* decryptedString;

// start time and end time initiated to record the length of a user's game.
clock_t start_time, end_time;
double elapsed_time;	

// Converts a string to uppercase.
void convertToUpperCase(char* input){
        int len = answerSize;
        for(int i = 0; i < len; i++){
                input[i] = toupper(input[i]);
        }
}

// getPuzzle checks if quoteSize is 0 if so then initiate the file and create the linkedlIst. Then, generate an answer by calling the getRandQuote() method. set
// answerSize to the length of the string and return the answer.
char* getPuzzle(void) {
	if(quotesSize == 0)
		initiateFile();
	char* answer = getRandQuote();
	answerSize = strlen(answer);    
	return answer;
}

// printPuzzleMap shows the alphabet relative to the character positions of the answer. It prints uppercase if they match, lowercase otheriwse.
// printPuzzleMap is used for two different "modes": one where it is used by displayworld and the other used for inputProccessor. When toggle isn't
// zero, it will not print the decrypted map. Otherwise, if 1, then it will.
bool printPuzzleMap(int toggle){
 int size = 0;
        char fin[answerSize];
        for(int i = 0; i < answerSize; i++){
                char cap = toupper(game.answer[i]);
                int pos = cap - 'A';
                if(isalpha(game.answer[i]) == 0){
                        fin[i] = game.answer[i];
                        size++;
                }
        	else if(isalpha(userAlphabet[pos])){
                	fin[i] = userAlphabet[pos];
			if(userAlphabet[pos] == cap)
                		size++;
			else
				fin[i] = tolower(userAlphabet[pos]);
        	}
        	else
                	fin[i] = '_';
        }
        fin[answerSize] = '\0';
	if(toggle == 0){
                sprintf(decryptedString, "Decrypted (Lower case letters are incorrect!):\n%s\n", fin);
	printf("%s\n",decryptedString);
	}
	if(size == answerSize){
                return true;
        }
        return false;
}

// initialization starts the game and retrieved the answer from getPuzzle(). It then encrypts the answer with "encryptPuzzle" and prints the encrypted answer.
int initialization(){
	game.answer = getPuzzle();
	shuffle(alphabet);
	game.puzzle = encryptPuzzle(game.answer);
	printf("%s\n", game.answer);
	//printf("Encrypted:\n%s\n", game.puzzle);
	//printPuzzleMap(0);
	start_time = clock();
	return 0;
}

// acceptInput prints a prompt and accepts a max of 5 characters from the user input.
/*char* acceptInput(char* input){
	printf("%s\n",input);
	convertToUppercase(input);	
	
}*/

// validCheck checks to see if the input is appropriate that the program can use to modify the userGuess.
// It checks to see if the length is not equal to 3 and that bpth characters are valid. If it doesn't then it prints an error message and returns false.
// Otherwise, it returns true.
bool validCheck(char* input){
        int len = strlen(input);
        char *firstChar = &input[0];
        char *secondChar = &input[1];
	if(strcmp(input, "QUIT") != 0){
        if(firstChar == NULL || secondChar == NULL || len != 3 || input == NULL){
                printf("Please enter a valid input! Ex: \'a\' \'b\' | \'quit\'\n");
                return false;
                }
	}
        return true;
}

// getAnswerVal calculates the "real" alphabetical value. It does this by searching the letter and returning its distance from 'A'. 
int getAnswerVal(char c){
	int i = 0;
	while(i < answerSize){
		char cap = toupper(game.answer[i]);
		if(cap == c){
			int pos = cap - 'A';
			return pos;
		}
		i++;
	}
	return -1;
}
// inputProcessor iterates the alphabet to find and replace characters. If the character is null then it grabs from the encrypted alphabet. If the character is not null then it
// will check to see if its in the correct space by calling getAnswerVal. If the iteration of the target is not equal to getAnswerVal then replace it.
bool inputProcessor(char target, char replace){
	printf("Target: %c, replace: %c\n", target, replace);
	bool noExist = false;
	int indexSaver = 0;
	for(int i  = 0; i < 26; i++){
		if(isalpha(userAlphabet[i]) == 0){
			char c = alphabet[i];
			if(c == target){
				indexSaver = i;
				noExist = true;
			}
		}
		else if(isalpha(userAlphabet[i]) != 0 && userAlphabet[i] == target){
			if(i != getAnswerVal(userAlphabet[i])){
				userAlphabet[i] = replace;
				break;
			}
		}
	}
	if(noExist == true)
		userAlphabet[indexSaver] = replace;
        if(printPuzzleMap(1))
                return true;
        return false;
}

char* acceptInput(char* input){
        convertToUpperCase(input);
	printf("%s\n",input);
        //convertToUpperCase(input);
	inputProcessor(input[11],input[12]);
}

// updateState compares the input to 'quit' in cases where the user wants to quickly end the game (it will return true in this case). It also checks if the input is valid with "validCheck", 
// a method delcared in "inputFormatter.c". If the script userGuess and answer are equal then return true. It returns false otherwise.
/*bool updateState(char* input){
	if(strcmp(input, "QUIT") == 0)
		return true;
	if(validCheck(input)){
	if(inputProcessor(input[0], input[1]))
		return true;
	}
	return false;
}*/

// displayWord simply displays the user's current guess.
char* displayWorld(){
	printf("Encrypted:\n%s\n", game.puzzle);
	decryptedString = malloc(600);
	printPuzzleMap(0);
	char* message = malloc(1500);
	sprintf(message, "HTTP/1.1 200 OK\r\ncontent-type: text/html; charset=UTF-8 \r\n\r\n<html><body>Encrypted: %s <P> Decrypted: %s\n <form submit=\"crypt\"><input type=\"text\" name=move autofocus maxlength=2> </input></form></html>", game.puzzle, decryptedString);
	printf("%s\n", message);
	free(decryptedString);
	return message;
}

// tearDown frees the allocated memmory to userGuess (because the encryptPuzzle method allocated memory to the return value), and the game ends.
int tearDown(void){
	end_time = clock();
	elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	free(game.puzzle);
	free(game.answer);
	resetAlphabet();
	resetUserAlphabet(userAlphabet);
	printf("All done!\nElapsed time: %.2f seconds\n", elapsed_time);
	answerSize = 0;
	return 0;
}

bool isGameOver(){
return false;
}

char* handleGame(char* input){
	int len = strlen(input);
	// crypt?move=
	if(len == 5){
		initialization();
		return displayWorld();
	}
	else if(len > 5){
		acceptInput(input);
		if(printPuzzleMap(1) == true){
			tearDown();
			return "You won!";
		}
		return displayWorld();
	}
	return "Error!";
} 
// gameLoop iterates through acceptInput, updateState, and displayWorld. It returns 0 when the "state" variable is false.
/*int gameLoop(){
	bool state = false;
	while(state == false){
		acceptInput();
		state = updateState();
		displayWorld();
	}
	return 0;
}*/

// main creates a loop governed by game. It will call initialization, gameLoop, and tearDown until game is false. It checks
// for user input depending on if the user wants to end the game or not.
/*int main(){
	bool game = true;
	while(game == true){
		initialization();
		gameLoop();
		tearDown();
		printf("Do you want to restart the game? Type \"y\" for yes and \"n\" for no.)\n");
		bool control = false;
		while(control == false){
		scanf("%s", input);
		if(input[0] == 'y')
			control = true;
		else if(input[0] == 'n'){
		freeQuotes();
		control = true;
		game = false;
		}
		else
			printf("Invalid input! Please enter \"\" or \"\"!\n");
		}
	}
	return 0;
}*/
