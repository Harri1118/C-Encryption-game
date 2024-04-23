#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

// Response opens the path, retrieves the size of the filepath, and forms a response using
// sprintf (to set up the 200 response and to display the content length). Aftet this, it
// then mallocs the sprintf product and a buffer is also malloced of the file contents.
// A third varaible is malloced, equal to the sum of the two sizes of the other strings.
// It then copies the 200 OK string and concatenates the next string. It then frees
// the two strings used for the final product and fin is returned.
char* response(char *input) {
    struct stat fileStatStruct;
    stat(input, &fileStatStruct);
    off_t fileSize = fileStatStruct.st_size;
    int file = open(input, O_RDONLY);
    char* begin = malloc(200);
    sprintf(begin, "HTTP/1.1 200 OK\r\n\r\nContent-Length:%ld\r\n\r\n", fileSize);
    int beginSize = strlen(begin);
    char* buffer = malloc(500);
    ssize_t bytesRead = read(file, buffer, 500);
    buffer[bytesRead] = '\0';
    close(file);
    int size = beginSize + 500;
    char *fin = malloc(size);
    strcpy(fin,begin);
    strcat(fin, buffer);
    free(begin);
    free(buffer);
    return fin;
}
