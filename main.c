#include <stdlib.h>
#include <cstdio>

int main (int argc, char **argv) {

    ush_loop();

    return EXIT_SUCCESS;
}

int ush_launcher(char ** args) {
    pid_t pid = fork();

    if (pid == 0) {
       if (execvp(args[0], args) == -1) {
        printf("ush_launcher_erorr: failed to load child")
       }
    } else if (pid > 0) {
        // inside parent process 
    } else {
        // error
    }
}

#define USH_TOK_BUFFISZE 64
#define USH_TOK_DELIM " \t\r\n\a"
char **ush_split_line(char *line) {
    int buffsize = USH_TOK_BUFFISZE, pos = 0;
    char **tokens = malloc(buffsize * sizeof(char*));
    char *token;

    // TODO Implement backslash, quoting, and escape sequencing.

    if (!tokens) {
        printf("ush_allocation_erorr in ush_split_line()")
        exit(EXIT_FAILURE);
    }

    token = strtok(line, USH_TOK_DELIM);
    while (token != NULL) {
        tokens[pos] = token;
        pos++;

        if (pos >= buffsize) {
            buffsize += USH_TOK_BUFFISZE;
            tokens = realloc(tokens, buffsize * sizeof(char*));

            if (!tokens) {
                printf("ush_allocation_error in ush_split_line()")
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, USH_TOK_DELIM);
    }
    tokens[pos] = NULL;
    return tokens;
}

#define USH_BUFFSIZE 1024
char *ush_read_line(void) {
    int buffsize = USH_BUFFSIZE;
    int pos = 0;
    char *buffer = malloc(sizeof(char)*buffsize);
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
        } else {
            buffer[pos] = c;
        }
        pos++;

        if (pos >= buffsize) {
            buffsize += USH_BUFFSIZE;
            buffer = realloc(buffer, buffsize);
            if (!buffer) {
                printf("ush_allocation_error");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void ush_loop(void) {
    char *line;
    char **args;
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