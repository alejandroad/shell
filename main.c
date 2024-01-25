#include <stdlib.h>
#include <cstdio>


/*
  Function Declarations for builtin shell commands:
 */
int ush_cd(char **args);
int ush_help(char **args);
int ush_exit(char **args);

/*
    List of builtin shell commands
*/
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &ush_cd,
    &ush_help,
    &ush_exit
};

int ush_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

int ush_cd(char **args) {
    if (args[1] == NULL) {
        printf("ERROR: ush: expected arguments to cd")
    } else {
        if (chdir(args[1]) != 0) {
            perror("ush");
        }
    }
    return 1;
}

int ush_help(char **args) {
    int i;
    printf("Alejandro's shell: ush\n")
    printf("Type program names and arguments, and then hit enter.\n")
    printf("The following are built into the shell:\n")

    for (i = 0; i < ush_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("For more information on other programs, use 'man'")

    return 1;
}

int exit(char **args) {
    return 0;
}

int ush_launcher(char **args) {
    pid_t pid = fork();
    int status;

    if (pid == 0) {
       if (execvp(args[0], args) == -1) {
        perror("ush");
       }
       exit(EXIT_FAILURE);
    } else if (pid > 0) {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    } else {
        perror("ush")
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

int main (int argc, char **argv) {

    ush_loop();

    return EXIT_SUCCESS;
}