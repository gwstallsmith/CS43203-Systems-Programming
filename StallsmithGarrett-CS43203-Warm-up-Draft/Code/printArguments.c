/* CS4-53203: Systems Programming \
/* Name: Garrett Stallsmith \
/* Date: 01/22/2024 \
/* AssignmentWarmup.txt\
*/


#include <stdio.h>

int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
    
    return 0;
}