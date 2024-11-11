#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp", "touch", "mkdir", "ls", "pwd", "cat", "grep", "chmod", "diff", "cd", "exit", "help"};

int isAllowed(const char *cmd)
{
    // TODO
    // return 1 if cmd is one of the allowed commands
    // return 0 otherwise

    // loop through each command
    for (int i = 0; i < N; i++)
    {
        // if find command in list return 1
        if (strcmp(cmd, allowed[i]) == 0)
        {
            return 1;
        }
    }
    // command not found return 0
    return 0;
}

int main()
{

    // TODO
    // Add variables as needed

    char *argumentsArray[21]; // array to hold arguments 
    pid_t pid; // process id place holder
    int code; // status code place holder

    char line[256];

    while (1)
    {
        fprintf(stderr, "rsh>");

        if (fgets(line, 256, stdin) == NULL)
            continue;

        if (strcmp(line, "\n") == 0)
            continue;

        line[strlen(line) - 1] = '\0';

        // TODO
        // Add code to spawn processes for the first 9 commands2
        // And add code to execute cd, exit, help commands
        // Use the example provided in myspawn.c
        
        // Initialize a counter for the number of command-line arguments
        int arguments = 0;
        
        // split input at the spaces and get ready to store in array
        argumentsArray[arguments] = strtok(line, " ");

        // Loop through each word to add to array don't go over 20 arguments because that is max
        while (argumentsArray[arguments] != NULL && arguments < 20)
        {
            arguments++; 
            argumentsArray[arguments] = strtok(NULL, " "); // Store next value
        }

        // if no commands are entered skip
        if (argumentsArray[0] == NULL)
            continue; 

        // call isAllowed to check if commands are allowed
        if (!isAllowed(argumentsArray[0]))
        {
            printf("NOT ALLOWED!\n");  
            continue; // skip if command is not allowed
        }

        // if user enters exit end program
        if (strcmp(argumentsArray[0], "exit") == 0)
        {
            return 0; 
        }
        // handle help command
        else if (strcmp(argumentsArray[0], "help") == 0)
        {
            printf("The allowed commands are:\n");
            for (int i = 0; i < N; i++)
            {
                printf("%s\n", allowed[i]); // show allowed commands
            }
            continue; // skip to next command entry
        }
        // cd command
        else if (strcmp(argumentsArray[0], "cd") == 0)
        {
            // only allow one argument after cd
            if (arguments > 2)
            {
                printf("-rsh: cd: too many arguments\n");
            }
            else
            {
                // try to change directory if error throw error
                if (chdir(argumentsArray[1]) != 0)
                {
                    perror("rsh: cd");
                }
            }
            continue; // skip to next command entry
        }

        // posix_spawnp fails print an error
        if (posix_spawnp(&pid, argumentsArray[0], NULL, NULL, argumentsArray, environ) != 0)
        {
            perror("rsh: posix_spawnp"); 
            continue; // skip to next command entry
        }

        // if spawn process fails
        if (waitpid(pid, &code, 0) == -1)
        {
            perror("rsh: waitpid");
        }
    }
    return 0;
}