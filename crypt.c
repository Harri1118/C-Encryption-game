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

// userAlphabet which will record the user input.
char userAlphabet[26];

// answerSize defines the character size of the answer.
int answerSize = 0;

// decryptedString is a variable used to pass onto variable "RESPONSE" in main.
char* decryptedString;

// start time and end time initiated to record the length of a user's game.
clock_t start_time, end_time;
double elapsed_time;	

// Converts a string to uppercase.
void convertToUpperCase(char* input){
        int len = strlen(input);
        for(int i = 0; i < len; i++){
                input[i] = toupper(input[i]);
        }
}

// getPuzzle checks if quoteSize is 0 if so then initiate the file and create the linkedlIst. Then, generate an answer by calling the getRandQuote() method. set
// answerSize to the length of the string and return the answer.
char* getPuzzle(void) {
	char* answer = getRandQuote();
	if(answer == NULL)
		return "Failed.";
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
	start_time = clock();
	return 0;
}

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

// acceptInput takes in the input, converts the characters within input, and then puts it through inputProcessor to modify the encrypted alphabet.
void acceptInput(char* input){
	if(strlen(input) >= 13){
        	convertToUpperCase(input);
		inputProcessor(input[11],input[12]);
	}
}

// displayWord simply displays the user's current guess. It mallocs a string which shows the decrypted characters, modifies the decrypted string using printPuzzleMap, and then creates the message. It then appends decrypted string to the message var and then frees decryptedString.
char* displayWorld(){
	decryptedString = malloc(600);
	printPuzzleMap(0);
	char* message = malloc(1500);
	sprintf(message, "HTTP/1.1 200 OK\r\ncontent-type: text/html; charset=UTF-8 \r\n\r\n<html><body>Encrypted: %s <P> Decrypted: %s\n <form submit=\"crypt\"><input type=\"text\" name=move autofocus maxlength=2> </input></form></html>", game.puzzle, decryptedString);
	//printf("%s\n", message);
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
	answerSize = 0;
	return 0;
}

// isGameOver determines if the alphabet is decrypted. It returns true if so, else it returns false.
bool isGameOver(){
	if(printPuzzleMap(1) == true)
		return true;
return false;
}

// isBeginPath checks if the input is the beginning of a game by checking whether the hotlink is valid or not.
bool isBeginPath(char* input){
	int len = strlen(input);
	if(len == 5){
		if(strcmp(input, "crypt") == 0)
			return true;
	}
	return false;
}

// isGameMove determines whether the hotlink is a valid link to move characters around.
bool isGameMove(char* input){
// iterate through alphabet. Check if null.
// If null, return false.
for(int i = 0; i < 26; i++){
	if(userAlphabet[i] != '\0')
		return true;
}
return false;
}

// handleGame validates the input, calls acceptInput if valid. Will also call initialization if beginning of the game. If at the end of a game then it will teardown and return a form object.
char* handleGame(char* input){
	if(isBeginPath(input)){
		initialization();
		return displayWorld();
	}
	else if(isGameMove(input) == true){
		acceptInput(input);
		if(isGameOver() == true){
			tearDown();
			char* win = "HTTP/1.1 200 OK\r\ncontent-type: text/html; charset=UTF-8 \r\n\r\n<html><body>Congratulations! You solved it! <a href=\"crypt\">Another?</a></body></html>";
			return win;
		}
		return displayWorld();
	}
	return "HTTP/1.1 200 OK\r\ncontent-type: text/html; charset=UTF-8 \r\n\r\n<html><body>Game isn't initiated! Please <a href=\"crypt\">click this link to start it!</a></body></html>";
} 
