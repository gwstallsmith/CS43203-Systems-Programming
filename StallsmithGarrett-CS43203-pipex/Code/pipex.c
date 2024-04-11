#include <stdio.h>  // Include standard input-output library
#include <stdlib.h> // Include standard library
#include <unistd.h> // Include POSIX operating system API

void pipeline(char *programs[]) { // Define function pipeline taking an array of strings as argument
    int file_descriptors[2]; // Array to store file descriptors for pipe
    pid_t pid; // Process ID
    int i = 0; // Iterator variable

    // fd[0] is read end
    // fd[1] is write end

    while(programs[i] != NULL) {    // Loop until a null terminator is found in the array
        pipe(file_descriptors);     // Create a pipe

        if((pid = fork()) == -1) {      // Fork a child process
            perror("fork");             // Print an error message if fork fails
            return;                     // Exit the function
        } else if (pid == 0) { // Child process
            close(file_descriptors[1]);                 // Close the write end of the pipe
            dup2(file_descriptors[0], STDIN_FILENO);    // Redirect standard input to the read end of the pipe
            close(file_descriptors[0]);                 // Close the read end of the pipe

            // Execute pipe
            execlp(programs[i], programs[i + 1], NULL); // Execute the program specified in the array

        } else if(pid > 0) {            // Parent process
            close(file_descriptors[0]);                 // Close the read end of the pipe
            dup2(file_descriptors[1], STDOUT_FILENO);   // Redirect standard output to the write end of the pipe
            close(file_descriptors[1]);                 // Close the write end of the pipe
        }

        // Increment to move to the next program in the array
        i++; 
    }
}

int main(int argc, char *argv[]) {
    if(argc < 2) { // Check if there are enough command line arguments
        // Print usage message to standard error
        fprintf(stderr, "Usage: %s <program1> <program2> ...\n", argv[0]); 
        return -1; // Return with error code if argument count is not correct
    }

    pipeline(&argv[1]); // Call the pipeline function with program arguments

    return 0; // Return success code
}
