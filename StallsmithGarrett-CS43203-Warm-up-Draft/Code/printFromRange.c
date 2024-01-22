/* CS4-53203: Systems Programming \
/* Name: Garrett Stallsmith \
/* Date: 01/22/2024 \
/* AssignmentWarmup.txt\
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
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

    rewind(filePointer);

    char buffer[256];

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

    fclose(filePointer);
    return 0;
}