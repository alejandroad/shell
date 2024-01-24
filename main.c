#include <stdlib.h>
#include <cstdio>

int main (int argc, char **argv) {
    
    ush_loop();

    return EXIT_SUCCESS;
}

#define USH_BUFFSIZE 1024
char* ush_read_line(void) {
    int buffsize = USH_BUFFSIZE;
    int pos = 0;
    char* buffer = malloc(sizeof(char)*buffsize);
    int c;

    if (!buffer) {
        printf("ush_allocation_error");
        exit(EXIT_FAILURE);
    }

    while (1) {
        c = getchar();

        if (c == EOF || c == '\n') {
            buffer[pos] = '\0';
            return buffer;
        }
    }
}

void ush_loop(void) {
    char* line;
    char** args;
    int status;

    do {
        printf("> ");
        line = ush_read_line();
        args = ush_split_line();
        status = ush_execute_line();

        free(line);
        free(args);
    } while (status);
}