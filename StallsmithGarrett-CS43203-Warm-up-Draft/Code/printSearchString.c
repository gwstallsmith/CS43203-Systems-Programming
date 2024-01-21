#include <stdio.h>

int main() {
    FILE *filePointer;
    filePointer = fopen("beeMovieScript.txt", "r");

    if(!filePointer) {
        printf("File pointer failure\n");
        return 1;
    }

    char userString[128];

    printf("Enter a string to search for: ");

    scanf("%99[^\n]", userString);

    printf("Searching for: %s\n", userString);

    char word[128];

    while(fscanf(filePointer, "%99s", word) == 1) {
        printf("%s\n", word);
    }

    fclose(filePointer);
    return 0;
}