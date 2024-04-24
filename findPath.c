#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

// getPath retrieves the filepath from the input head. It returns
// the result afterwards.
char *getPath(char input[1500]){
int i = 5;
int n = 0;
char Path[100];
while(input[i] != ' '){
Path[n] = input[i];
n++;
i++;
}
Path[n] = '\0';
char *fin = strdup(Path);
return fin;
}

bool isEndingResponse(char* input){
	char* win = "HTTP/1.1 200 OK\r\ncontent-type: text/html; charset=UTF-8 \r\n\r\n<html><body>Congratulations! You solved it! <a href=\"crypt\">Another?</a></body></html>";
	char* err = "HTTP/1.1 200 OK\r\ncontent-type: text/html; charset=UTF-8 \r\n\r\n<html><body>Game isn't initiated! Please <a href=\"crypt\">click this link to start it!</a></body></html>";
	int len = strlen(input);
	int winLen = strlen(win);
	int errLen = strlen(err);
	if(len == winLen){
		if(strcmp(input, win) == 0)
			return true;
	}
	if(len == errLen){
		if(strcmp(input, err) == 0)
			return true;
	}
	return false;
}

// pathExists filters the header so that the filepath can be extracted
// from it. It then calls fileExists to validate whether the file
// exists or not.
bool pathExists(char input[1500]){
        char* crypt = "crypt";
	for(int i = 0; i < 5; i++){
		if(input[i] != crypt[i])
			return false;
	}
	return true;
}
