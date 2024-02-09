#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <stdbool.h>
#include <pwd.h>
#include <sys/types.h>

#include <ctype.h>
#include <string.h>
#include <unistd.h>
//#include "utmplib.c"

bool is_number(char number[]) {
    for(int i = 0; i < strlen(number); i++) {
        if(!isdigit(number[i])) {
            return false;
        }
    }
}

char **get_logged_in(char *log_names[], int length, int *index_pointer) {
    char **logged_in;
    logged_in = (char**)malloc(sizeof(char*));

    utmp_open(UTMP_FILE);

    struct utmp *utmp_file_descriptor;
    //utmp_file_descriptor = utmp_open(UTMP_FILE);
    utmp_file_descriptor = utmp_next();

    int index = *index_pointer;

    while(utmp_file_descriptor != NULL) {
        for(int i = 0; i < length; i++) {
            if(strncmp(log_names[i], utmp_file_descriptor->ut_name, strlen(log_names[i])) == 0) {
                logged_in = realloc(logged_in, (index + 1)*sizeof(char*));
                logged_in[index] = log_names[i];
                index++;
            }
        }
        utmp_file_descriptor = utmp_next();
    }

    *index_pointer = index;

    utmp_close();

    return logged_in;
}

void compare_report(char **lhs, int lhs_length, char **rhs, int rhs_length) {
    for(int i = 0; i < lhs_length; i++) {
        bool marked = false;
        for(int j = 0; j < rhs_length; j++) {
            if(rhs[j] == lhs[i]) {
                marked = true;
            }
        }
        if(!marked) {
            printf("%s logged out\n", lhs[i]);
        }
    }

    for(int j = 0; j < rhs_length; j++) {
        bool marked = false;
        for(int i = 0; i < lhs_length; i++) {
            if(lhs[j] == rhs[i]) {
                marked = true;
            }
        }
        if(!marked) {
            printf("%s logged in\n", rhs[j]);
        }
    }
}

bool check_program_exit(char **logged_in, int length) {
    uid_t id = geteuid();

    struct passwd* password_buffer = getpwuid(id);

    bool marked = false;

    for(int i = 0; i < length; i++) {
        if(strncmp(logged_in[i], password_buffer->pw_name, strlen(password_buffer->pw_name)) == 0) {
            marked = true;
        }
    }

    return marked;
}

int main(int argc, char *argv[]) {
    if(argc > 1) {
        int sleep_time = 300;
        int position = 1;

        if(is_number(argv[1])) {
            sleep_time = atoi(argv[1]);
            position = 2;
        }

        char *lognames[argc - position];
        
        for(int i = 0; i < argc - position; i++) {
            lognames[i] = argv[i + position];
        }

        int length_old = 0;
        
        char **logged_in_old = get_logged_in(lognames, argc - position, &length_old);

        for(int i = 0; i < length_old; i++) {
            printf("%s ", logged_in_old[i]);
        }
        
        printf("are currently logged in\n");

        while(check_program_exit(logged_in_old, length_old)) {
            printf("...\n");
            sleep(sleep_time);

            int length_new = 0;

            char **logged_in_new = get_logged_in(lognames, argc - position, &length_new);

            compare_report(logged_in_old, length_old, logged_in_new, length_new);

            logged_in_old = realloc(logged_in_old, sizeof(logged_in_new));
            logged_in_old = logged_in_new;

            length_old = length_new;

            for(int i = 0; i < length_old; i++) {
                printf("%s ", logged_in_old[i]);
            }
            
            putchar('\n');


        }


    } else {
        printf("Too few arguments provided.\nUsage: ./Watch <sleep_time> <user1> <user2> <userN>\n");
    }

    return 0;
}