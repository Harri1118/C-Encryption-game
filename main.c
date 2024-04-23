#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "crypt.c"
#include "findPath.c"
#include "loadPath.c"

void *userHandler(void *arg){
    int cliSocket = *(int*)arg;
    char buffer[1500];
    ssize_t byteStore = read(cliSocket, buffer, 1500 - 1);
    //printf("%s\n", buffer);
    if (byteStore < 0) {
        perror("read");
        close(cliSocket);
        return NULL;
    }
    buffer[byteStore] = '\0';
    char* notFound = "404 Not found.";
    char *path = getPath(buffer);
    //printf("%s\n", path);
    if(pathExists(path) == false){
    	send(cliSocket, notFound, strlen(notFound), 0);
    }
    else{
    	char* RESPONSE = handleGame(path);
	
	//printf("%s",RESPONSE);
        send(cliSocket, RESPONSE, strlen(RESPONSE), 0);
        //free(RESPONSE);
    }
    free(path);
    close(cliSocket);
    return NULL;
}

int main(){
    int serverFd;
    int cliSocket;
    int opt = 1;
    struct addrinfo hints, *serverInfo, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, "8000", &hints, &serverInfo);
    for (p = serverInfo; p != NULL; p = p->ai_next) {
        serverFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        bind(serverFd, p->ai_addr, p->ai_addrlen);
        break;
    }
        freeaddrinfo(serverInfo);
    if (listen(serverFd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    loadPuzzles();
    while(1){
        int cliSocket = accept(serverFd, NULL, NULL);
        pthread_t thread;
        pthread_create(&thread, NULL, userHandler, &cliSocket);
        pthread_detach(thread);
    }
    return 0;
}
