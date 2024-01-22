#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if(argc != 2) {
        perror("Incorrect number of arguments\n");
        return 1;
    }

    printf("Reading file: %s\n", argv[1]);

    FILE *filePointer;
    filePointer = fopen(argv[1], "r");

    if(!filePointer) {
        printf("File pointer failure\n");
        return 1;
    }

    int standardInput = dup(fileno(stdin));
    dup2(fileno(filePointer), fileno(stdin));

    int character;

    int charCount = 0;
    int wordCount = 0;
    int lineCount = 0;

    while((character = getchar()) != EOF) {
        //printf("%c", character);

        charCount++;
        
        if(character == 32) {
            wordCount++;
        }
        
        if(character == 10) {
            lineCount++;
        }

    }

    printf("\nCharacter count: %d\n", charCount);
    printf("Word count: %d\n", wordCount);
    printf("Line count: %d\n", lineCount);


    dup2(standardInput, fileno(stdin));
    fclose(filePointer);


    return 0;
}