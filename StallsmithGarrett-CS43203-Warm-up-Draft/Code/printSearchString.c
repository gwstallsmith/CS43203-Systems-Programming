#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
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

    char userString[128];

    printf("Enter a string to search for: ");

    scanf("%99[^\n]", userString);

    int stringLen = strlen(userString);

    printf("String Length in bytes: %d\n", stringLen);

    printf("Searching for: %s\n", userString);


    char word[stringLen];

    int printFlag;

    while(fscanf(filePointer, "%99s", word) == 1) {
        printFlag = 1;
        for(int i = 0; i < stringLen; i++) {
            if(word[i] != userString[i]) {
                printFlag = 0;
            }
        }
        if(printFlag) {
            printf("%s\n", word);
        }
    }

    fclose(filePointer);
    return 0;
}