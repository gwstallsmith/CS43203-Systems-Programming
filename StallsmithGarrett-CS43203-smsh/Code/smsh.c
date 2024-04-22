#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE_SIZE 1024
#define MAX_AGUMENTS 1024

// Read commands found in Shell
void read_command(char *command) {
    // Pass command as pointer so we can edit memory address with no need to return a value

    // Get command from standard input
    // As long as command is smaller than MAX_LINE_SIZE
    fgets(command, MAX_LINE_SIZE, stdin);

    // Searches for first instance of newline character ("\n")
    // Knowing this, we will know where the command ends
    // We now know the entire command
    // Remove the newline for execution by replacing it with a null terminator
    command[strcspn(command, "\n")] = '\0';
}

// Parse all arguments found in command
void parse_command(char *command, char *args[]) {
    char *token;    // Define character for tokenization
    int itr = 0;    // Define iterator integer to move throughout character array

    // Split command into argument tokens on " " (whitespace) characters
    // Get first argument as a token
    token = strtok(command, " ");

    // While there are arguments to tokenize
    while(token != NULL) {          // strtok returns null if no string is found to tokenize
        args[itr++] = token;        // Store token pointer in argument pointer array
        token = strtok(NULL, " ");  // Continue tokenizing from null terminator of last token
    }

    args[itr] = NULL;   // Store null terminator in last index of argument array

}

// Execute command and its arguments
void execute_command(char *args[]) {
    if(args[0] == NULL) { return; }     // If there is no command return out


    // First, check for built in Unix commands
    if(strcmp(args[0], "exit") == 0 ) {         // Check for exit shell command
        // If command is exit,
        // then exit the current program (shell)
        exit(EXIT_SUCCESS);

    } else if(strcmp(args[0], "cd") == 0) {     // Check for change directory command
        // If no arguments for change directory,
        // then change directory to home directory
        if(args[1] == NULL) {
            // Change directory to home directory of the system
            chdir(getenv("HOME"));
        } else {    
            // Attempt to change directory to directory provided in arguments
            // If not successful,
            // then an error occurred
            if(chdir(args[1]) != 0) {
                perror("cd");
            }
        } 
    } else if(strcmp(args[0], "read") == 0) {   // Check for new read input command
        // Act like Bourne shell read command

        // Create string (char array) the size of the shell line
        char variable[MAX_LINE_SIZE];

        // Print first argument
        printf("%s", args[1]);

        // Get user input from shell and store in variable char array
        fgets(variable, MAX_LINE_SIZE, stdin);

        // Remove newline character and replace with null terminator
        variable[strcspn(variable, "\n")] = '\0';

        // Set environment ot variable specifications
        setenv(args[1], variable, 1);

    } else {    // Otherwise we are running arbitrary programs
        // Fork to execute arbitrary program
        pid_t pid = fork();

        // If not successful, an error has occurred
        if(pid < 0) {
            perror("fork");

        } else if(pid == 0) {   // Successful fork
            // Run child process
            if(execvp(args[0], args) < 0) { // If not successful, an error has occurred
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } else {
            // Wait for child to finish after execution
            wait(NULL);
        }
    }

}

int main(int argc, char *argv[]) {
    // Define character arrays to store shell commands and arguments
    char command[MAX_LINE_SIZE];

    // String array instead of char array (string) for multiple arguments
    char *args[MAX_AGUMENTS];

    if(argc >= 1) {
        // If a file is provided,
        // then read file and execute commands in files

        // Open script file
        FILE *script = fopen(argv[1], "r");

        // If script doesn't exist, an error occurred
        if(!script) {
            perror("Script file error");
            exit(EXIT_FAILURE);
        }
        
        // Read through script file while there are lines (commands) to execute
        while(fgets(command, MAX_LINE_SIZE, script) != NULL) {

            printf("RUNNING: %s\n\n", command);
            // For each command found in script file,
            // Parse for command and arguments, then execute

            parse_command(command, args);
            execute_command(args);
        }

        // Close script file before exiting
        fclose(script);
    } else {    // No script file provided
        // Read commands from standard input while shell is open
        while(1) {
            // Print shell name
            printf("smsh$ ");

            // Do shell behavior

            // Read command into string for processing 
            read_command(command);

            // Parse for command and arguments, then execute
            parse_command(command, args);

            // After parsing, run command
            execute_command(args);

        }
    }

    return 0;
}