/* CS4-53203: Systems Programming \
/* Name: Garrett Stallsmith \
/* Date: 01/22/2024 \
/* AssignmentWarmup.txt\
*/


#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    // We are providing a file on the command line.
    // If we receive anything other than two arguments, the program doesn't work as intended
    if(argc != 2) {
        perror("Incorrect number of arguments\n");
        return 1;
    }

    printf("Reading file: %s\n", argv[1]);

    FILE *filePointer;                    // Generate file pointer to read through the file
    filePointer = fopen(argv[1], "r");    // Open the file provided as an argument in read mode

    // If we can't generate a file pointer, the program doesn't work as intended
    if(!filePointer) {
        printf("File pointer failure\n");
        return 1;
    }

    int standardInput = dup(fileno(stdin));     // We save the standard input because we are about to edit it
    dup2(fileno(filePointer), fileno(stdin));   // We change the standard input to the filePointer to use getchar within our provided file

    int character;

    int charCount = 0;
    int wordCount = 0;
    int lineCount = 0;

    while((character = getchar()) != EOF) {
        charCount++;
        
        if(character == 32) {   // 32 is ' ' (blank space)
            wordCount++;
        }
        
        if(character == 10) {   // 10 is '\n' (new line)
            lineCount++;
        }

    }

    printf("\nCharacter count: %d\n", charCount);
    printf("Word count: %d\n", wordCount);
    printf("Line count: %d\n", lineCount);


    dup2(standardInput, fileno(stdin));     // Return standard input to original state
    fclose(filePointer);    // Safely close the file pointer


    return 0;
}