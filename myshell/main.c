#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 100

void parse(char *input, char **args) {
    int i = 0;
    char *token = strtok(input, " \t\n");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}

int main() {
    char input[MAX_LINE];
    char *args[MAX_ARGS];

    while (1) {
        printf("myShell> ");
        fflush(stdout);

        if (fgets(input, MAX_LINE, stdin) == NULL) {
            printf("\n");
            break;
        }

        // Remove empty input
        if (strlen(input) <= 1) continue;

        parse(input, args);

        // Built-in: exit
        if (strcmp(args[0], "exit") == 0) {
            printf("Exiting shell...\n");
            break;
        }

        // Built-in: cd
        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                printf("myShell: expected argument to \"cd\"\n");
            } else {
                if (chdir(args[1]) != 0) {
                    perror("myShell");
                }
            }
            continue;
        }

        // Fork and execute
        pid_t pid = fork();

        if (pid == 0) {
            // Child process
            if (execvp(args[0], args) == -1) {
                perror("myShell");
            }
            exit(EXIT_FAILURE);
        } 
        else if (pid < 0) {
            perror("myShell");
        } 
        else {
            // Parent waits
            wait(NULL);
        }
    }

    return 0;
}