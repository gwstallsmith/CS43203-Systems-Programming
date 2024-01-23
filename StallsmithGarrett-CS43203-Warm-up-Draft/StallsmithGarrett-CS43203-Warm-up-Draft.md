**X [1]** In the following code, the first printf() reached produces the output “14” but the second printf() can cause a bus error or segmentation fault. Why? 

```
main () {
    int *p;
    funct ( p);
    printf(“%d\n” , *p) ;
}

funct (int *p2) {
    p2=( int *)malloc(4);
    *p2=14;
    printf (“%d\n” , *p2) ;
} 
```
In funct p2 is passed by value rather than reference. p2 is then assigned  as an integer pointer with four bytes allocated to store the integer 14. When funct returns to the main block p is not the same as p2 (pass by value not reference). So the pointer p was never allocated four bytes to store integer 14 in. When the final printf is called it tries to dereference a pointer that points to no memory.

<hr>

**X [2]** Write a C program that reads a text file and prints out any words that begins with a user given string. the filename should be given at the command line as an argument. the program should prompt the user for the search string. the program should then read the file one word at a time and front out the word if its first N bytes match the search string, where N is the length of the search string.

```
#include <stdio.h>
#include <string.h>

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
```

<hr>

**X [3]** Explain the purpose of the following Unix commands: ls, cat, rm, cp, mv, mkdir, cc

* ls
    * ls lists the files within a directory
* cat
    * cat concatenates files together
    * cat can also be used to print the contents of a file to the output stream (usually shell)
* rm
    * rm permanently removes data (files, directories, etc.)
* cp
    * cp creates a copy of data and stores the copy in a specified directory
* mv
    * mv changes the pointer of the file to change its directory
    * In essence, it moves data from one location to another
        * Renaming files using mv works this way as well
* mkdir
    * mkdir creates a new directory
* cc
    * cc compiles a C file and creates an executable file

<hr>

**X [4]** Using your favorite editor, create a small text file. Use cat to create another file consisting of five repetitions of this small text file. Use wc to count the number of characters and words in the original file and in the one you made from it. Explain the result. Create a subdirectory and move the two files into it.

```
$ cat Q4SmallFile.txt Q4SmallFile.txt Q4SmallFile.txt Q4SmallFile.txt > repeated.txt
$ wc Q4SmallFile.txt 
 0  8 37 Q4SmallFile.txt
$ wc repeated.txt 
  0  29 148 repeated.txt
$ mkdir Sub-Dir
$ mv Q4SmallFile.txt Sub-Dir/
$ mv repeated.txt Sub-Dir/

```

cat concatenated the data of Q4SmallFile.txt four times and stored in a text file called repeated.txt

repeated.txt stores the following:
```
Small text file with some data in it.Small text file with some data in it.Small text file with some data in it.Small text file with some data in it.
```

<hr>

**X [5]** Write, compile, and execute a C program that prints a welcoming message of your choice.

```
#include <stdio.h>

int main() {
    printf("Welcome to CS43203 - Systems Programming.\n");
    return 0;
}
```
```
$ g++ -o WelcomeMessage.out welcomeMessage.c 
$ ./WelcomeMessage.out 
Welcome to CS43203 - Systems Programming.
```
<hr>

**X [6]** Write, compile, and execute a C program that prints its arguments.
```
#include <stdio.h>

int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
    
    return 0;
}
```
```
$ g++ -o PrintArgs.out printArguments.c
$ ./PrintArgs.out I am arguing with the command line
./PrintArgs.out
I
am
arguing
with
the
command
line

```
<hr>

**X [7]** Using getchar() write a program that counts the number of words, lines, and characters in its input.

```
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
```
<hr>

**X [8]** Create a file containing a C function that prints the message "hello, world". Create a separate file containing the main program which calls this function. Compile and link the resulting program, calling it hw.
```
#ifndef HW_H
#define HW_H

void helloWorld();

#endif
```
```
#include "hw.h"
#include <stdio.h>

void helloWorld() {
    printf("hello, world\n");
}
```
```
#include "hw.h"

int main() {
    helloWorld();
    return 0;
}
```
```
$ g++ mainhw.c hw.c -o HW.out
$ ./HW.out 
hello, world
```

<hr>

**X [9]** Look up the entries for the following topics in your system’s manual; the cat command, the printf function, and the write system call.

<hr>

**X [10]** Write a function that computes some basic statistics for a list of numbers and stores those results in parts of a struct. In particular, given this definition:
```
struct numlist {
    float *list;    /* points to list of numbers */
    int len;        /* number of items in list */

    float min,      /* the minimal value in list */
    max,            /* the maximal value in list */
    avg;            /* the mean of the numbers */
};
```

write a function compute_stats(struct numlist *listptr) that takes as an argument a pointer to a struct numlist with list and len already initialized and computes and fills in the other three members

<hr>

**[11]**
* **X Part 1** - Write a program that prints a range of lines from a text file. The program should take command line arguments of the form:
* **X Part 2** - Write a program called last10 that prints the last ten lines of a text file. The pro- gram can be used from the command line with:
    * last10 filename or
    * last10
    * If there is no filename, last10 processes standard input. 

