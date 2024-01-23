/* CS4-53203: Systems Programming \
/* Name: Garrett Stallsmith \
/* Date: 01/22/2024 \
/* AssignmentWarmup.txt\
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    // We expect a file, a left bound, and a right bound
    // If we do not get four arguments the program may not work correctly
    if(argc != 4) {
        perror("Incorrect number of arguments\n");
        return 1;
    }

    printf("Searching file: %s\n\n", argv[1]);

    FILE *filePointer;
    filePointer = fopen(argv[1], "r");

    if(!filePointer) {
        printf("File pointer failure\n");
        return 1;
    }

    // Define user range
    int leftBound = atoi(argv[2]);
    int rightBound = atoi(argv[3]);
    int lineNumber = 1;

    // Need character array to store each line
    char buffer[256];

    // If line is within user specified range, print (along with the line number for good measure)
    while (fgets(buffer, sizeof(buffer), filePointer) != NULL) {
        if (lineNumber >= leftBound && lineNumber <= rightBound) {
            printf("%i\t", lineNumber);
            printf("%s", buffer);
        }

        lineNumber++;

        // Stop reading lines if we have reached the end of the range
        if (lineNumber > rightBound) {
            break;
        }
    }

    // Close file gracefully
    fclose(filePointer);
    return 0;
}