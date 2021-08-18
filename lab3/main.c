/* 
 * File:   main.c
 * Author: Jeff
 *
 * Created on February 3, 2019, 6:03 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define SHELL_INPUT_SIZE 1024
#define SHELL_TOK_SIZE 64
#define SHELL_TOK_DELIM " \t\r\n\a"

int shell_cd(char **args);
int shell_help(char ** args);
int shell_exit(char ** args);

char *builtin_str[] = { "cd", "help", "exit" };
int (*builtin_func[]) (char**) = { &shell_cd, &shell_help, &shell_exit };

int shell_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int shell_cd(char **args){
    if (args[1] == NULL) {
        fprintf(stderr, "shell: expected argument to \"cd\"\n");
    }
    else {
        if (chdir(args[1]) != 0) {
            perror("shell");
        }
    }
    return 1;
}

int shell_help(char** args){
    int i;
    printf("Jeff's Shell\n");
    printf("The following are built in functions:\n");
    
    for (i = 0; i < shell_num_builtins(); i ++) {
        printf(" %s\n", builtin_str[i]);
    }
    return 1;
}

int shell_exit(char **args) {
    return 0;
}

int shell_launch(char **args){
    pid_t pid, wpid;
    int status;
    
    pid = fork();
    
    if (pid == 0) {
        if (execvp(args[0], args) == -1){
            perror("shell");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0) {
        perror("shell");
    }
    else{
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int shell_execute(char **args) {
    int i;
    
    if (args[0] == NULL){
        return 1;
    }
    
    for(i = 0; i < shell_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }
    return shell_launch(args);
}

char *shell_read(void) {
    int inputsize = SHELL_INPUT_SIZE;
    int position = 0;
    char* buffer = malloc(sizeof(char) * inputsize);
    int input;
    
    if(!buffer){
        fprintf(stderr, "shell: allocation error\n");
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        input = getchar();
        
        if (input == EOF || input == '\n') {
            buffer[position] = '\0';
            return (char*)buffer;
        }
        else {
            buffer[position] = input;
        }
        position++;
    }
    
    if (position >= inputsize) {
        inputsize += SHELL_INPUT_SIZE;
        buffer = realloc(buffer, inputsize);
        if(!buffer) {
            fprintf(stderr, "shell: allocation error\n");
            exit(EXIT_FAILURE);
        }
    }
}

char **shell_split(char* input) {
    int inputsize = SHELL_TOK_SIZE, position = 0;
    char** tokens = malloc(inputsize * sizeof(char**));
    char* token;
    
    if(!tokens) {
        fprintf(stderr, "shell: allocation error\n");
        exit(EXIT_FAILURE);
    }
    
    token = strtok(input, SHELL_TOK_DELIM);
    while(token != NULL){
        tokens[position] = token;
        position++;
        
        if(position >= inputsize) {
            inputsize += SHELL_TOK_SIZE;
            tokens = realloc(tokens, inputsize * sizeof(char*));
            if(!tokens) {
                fprintf(stderr, "shell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, SHELL_TOK_DELIM);
    }
    tokens[position] = NULL;
    return (char**)tokens;
}

void shell_loop(void){
    char *input;
    char **args;
    int status;
    
    do {
        printf("Your command> ");
        input = shell_read();
        args = shell_split(input);
        status = shell_execute(args);
        
        free(input);
        free(args);
    } while (status);
}

int main(int argc, char** argv) {
    
    shell_loop();
    
    return EXIT_SUCCESS;
}

