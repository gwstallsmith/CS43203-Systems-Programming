**[1]** In the following code, the first printf() reached produces the output “14” but the second printf() can cause a bus error or segmentation fault. Why? 

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

**[2]** Write a C program that reads a text file and prints out any words that begins with a user given string. the filename should be given at the command line as an argument. the program should prompt the user for the search string. the program should then read the file one word at a time and front out the word if its first N bytes match the search string, where N is the length of the search string.

```
Code: printSearchString.c
Executable: PrintSearchString.out
Usage: ./PrintSearchString.out fileToSearch.txt userSearchString
```

<hr>

**[3]** Explain the purpose of the following Unicommands: ls, cat, rm, cp, mv, mkdir, cc

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

**[4]** Using your favorite editor, create a small text file. Use cat to create another file consisting of five repetitions of this small text file. Use wc to count the number of characters and words in the original file and in the one you made from it. Explain the result. Create a subdirectory and move the two files into it.

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

**[5]** Write, compile, and execute a C program that prints a welcoming message of your choice.

```
Code: welcomeMessage.c
Executable: WelcomeMessage.out
Usage: ./WelcomeMessage.out
```
<hr>

**[6]** Write, compile, and execute a C program that prints its arguments.

```
Code: printArguments.c
Executable: PrintArguments.out
Usage: ./PrintAguments.out argToPrint1, argToPrintN
```
<hr>

**[7]** Using getchar() write a program that counts the number of words, lines, and characters in its input.

```
Code: getCharPrint.c
Executable: GetCharPrint.out
Usage: ./GetCharPrint.out fileToSearch.txt
```
<hr>

**[8]** Create a file containing a C function that prints the message "hello, world". Create a separate file containing the main program which calls this function. Compile and link the resulting program, calling it hw.

```
Code: mainhw.c, hw.c, hw.h
Executable: HW.out
Usage: ./HW.out
```

<hr>

**[9]** Look up the entries for the following topics in your system’s manual; the cat command, the printf function, and the write system call.

<hr>

**[10]** Write a function that computes some basic statistics for a list of numbers and stores those results in parts of a struct. In particular, given this definition:
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

``` 
Code: structStats.c
Executable: StructStats.out
Usage: ./StructStats.out
```

<hr>

**[11]**
* **Part 1** - Write a program that prints a range of lines from a text file. The program should take command line arguments of the form:

```
Code: printFromRange.c
Executable: PrintFromRange.out
Usage: ./PrintFromRange.out fileToSearch.txt leftBound rightBound
```

* **Part 2** - Write a program called last10 that prints the last ten lines of a text file. The pro- gram can be used from the command line with:
    * last10 filename or
    * last10
    * If there is no filename, last10 processes standard input. 

```
Code: last10.c
Executable: Last10.out
Usage: ./Last10.out fileToSearch.txt
        or
       ./Last10.out
```

