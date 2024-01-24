/* CS4-53203: Systems Programming \
/* Name: Garrett Stallsmith \
/* Date: 01/22/2024 \
/* AssignmentWarmup.txt\
*/


#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    // We expect a file to be provided
    if(argc != 2) {
        perror("Incorrect number of arguments\n");
        return 1;
    }

    printf("Searching file: %s\n", argv[1]);

    FILE *filePointer;
    filePointer = fopen(argv[1], "r");

    if(!filePointer) {
        printf("File pointer failure\n");
        return 1;
    }

    // Character array for user string
    char userString[128];

    printf("Enter a string to search for: ");

    // Store user input into userString
    scanf("%99[^\n]", userString);

    // Figure out length
    // Used for substring search later
    int stringLen = strlen(userString);

    printf("String Length in bytes: %d\n", stringLen);

    printf("Searching for: %s\n", userString);

    // Need a character array the size of the user string for substring search
    char word[stringLen];

    int printFlag;

    while(fscanf(filePointer, "%99s", word) == 1) {
        printFlag = 1;
        // Print Flag is assumed true
        // Contradict the search substring to falsify
        for(int i = 0; i < stringLen; i++) {
            if(word[i] != userString[i]) {
                printFlag = 0;
            }
        }
        // If no contradictions found, substring is valid
        // Print that bad boy
        if(printFlag) {
            printf("%s\n", word);
        }
    }

    // Close file gracefully
    fclose(filePointer);
    return 0;
}