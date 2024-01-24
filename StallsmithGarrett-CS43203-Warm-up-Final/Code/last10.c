/* CS4-53203: Systems Programming \
/* Name: Garrett Stallsmith \
/* Date: 01/22/2024 \
/* AssignmentWarmup.txt\
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    FILE *filePointer;

    // If not given a file to read default to beeMovieScript.txt
    if(argc != 2) {
        filePointer = fopen("beeMovieScript.txt", "r");
        printf("Searching file: %s\n\n", "beeMovieScript.txt");

    } else {
        filePointer = fopen(argv[1], "r");
        printf("Searching file: %s\n\n", argv[1]);

    }


    if(!filePointer) {
        printf("File pointer failure\n");
        return 1;
    }

    int rightBound = 1;
    int bufferChar;

    // Calculate the (inclusive) right bound by incrementing every time a new line character is encountered
    while((bufferChar = fgetc(filePointer)) != EOF) {
        if(bufferChar == '\n') {
            rightBound++;
        }
    } 

    printf("File has %i lines\n", rightBound);

    // Subtract by ten to get left bound. Once we know right bound we know where the last ten line begin and end
    int leftBound = rightBound - 10;
    int loopCount = 0;
    // Need a character array to actually read each line
    char buffer[256];

    // Make file pointer point to the start of the file
    rewind(filePointer);

    // Get each line and cound the loop
    // If the loop count is between our left and right bounds, print the line (along with the line number for good measure)
    while(fgets(buffer, sizeof(buffer), filePointer) != NULL) {
        if (loopCount >= leftBound && loopCount <= rightBound) {
            printf("%i\t", loopCount);
            printf("%s", buffer);
        }

        loopCount++;

        // Stop reading lines if we have reached the end of the range
        if (loopCount > rightBound) {
            break;
        }

    }

    // Close the file gracefully
    fclose(filePointer);
    return 0;
}