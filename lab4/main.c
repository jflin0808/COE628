/* 
 * File:   main.c
 * Author: Jeff     
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

char input[100];
char cmd1[50];
char cmd2[50];

void parseInput()
{
    int j = 0;
    int counter1 = 0;
    int counter2 = 0;
    for (int i = 0; i < strlen(input)-1 ; i++)
    {
        if(input[i] != '|' &&  counter1 == 0)
        {
            cmd1[counter1] = input[i];
            counter1++;
        }
        else if(input[i] == '|')
        {
            j = 1;
            i++;
        }
        else 
        {
            cmd2[counter2] = input[i];
            counter2++;
        }
    }
}

int main(int argc, char** argv) {
    int fd[2], pid;
    
    printf("Your Command> ");
    fgets(input, 100, stdin);
    parseInput();
    
    pipe(fd); // fd[0] is input, fd[1] is output
    pid = fork();
    
    if(pid == 0)
    {
        dup2(fd[1], 1);
        close(fd[0]);
        system(cmd1);
        exit(0);
    }
    else
    {
        dup2(fd[0], 0);
        close(fd[1]);
        system(cmd2);
    }
    return (EXIT_SUCCESS);
}

