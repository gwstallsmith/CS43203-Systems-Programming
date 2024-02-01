/* 
CS4-53203: Systems Programming
Name: Garrett Stallsmith
Date: 1/21/2024
AssignmentWarm-up.txt
*/


#include	<stdio.h>
#include    <stdlib.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<utmp.h>
#include    <unistd.h>
#include    <string.h>

/*
Every five minutes (300 seconds) the program wakes up and checkes the utemp file

Compare the list of active users it finds to the list of active users if found last time. Need to make data structure to hold list of active users

Program reports when a user goesf rom one or more logins to no logins
Without this restritcion, each time a user opens a new terminal the program will notify

1.) Take one or more lognames as command line arguments. Also takes an argument for time.

*/
void dump_utmp_file(char* filename, int argc, char* argv[]);
void show_utmp_record(struct utmp* record);
char *get_utmp_type_name(int type_number);
void assign_snapshot(char *filename, struct utmp* snapshot);


int main(int argc, char* argv[]) {
    int sleep_timer = atoi(argv[1]);

    if(atoi(argv[1]) <= 0) {
        sleep_timer = 300;
    }
    if(argc < 2) {
        perror("Provide time per scan and user names you would like to scan");
        return 1;
    }
    if(argc < 3) {
        perror("Provide user names you would like to scan");
        return 1;
    }

    printf("Scaning for:\n");

    for(int i = 1; i < argc; i++) {
        if(i == 1) {
            printf("%i seconds\n\nFor Users:\n", sleep_timer);
        } else {
            printf("%s\n", argv[i]);
        }
        if(i == argc - 1) {
            printf("\n");
        }
    }






    while(1) {
        dump_utmp_file(UTMP_FILE, argc, argv);
        printf("...\n");
        sleep(sleep_timer);

    }



    return 0;
}

//
// utmp functions
//

void dump_utmp_file(char *filename, int argc, char* argv[]) { 
    struct utmp utmp_record;
    int file_descriptor;

    file_descriptor = open(filename, 0);

    if (file_descriptor == -1) {
        perror(filename);
        return;
    }



    int string_equal_flag;

    while (read(file_descriptor, &utmp_record, sizeof(utmp_record)) == sizeof(utmp_record)) {
        string_equal_flag = 1;
        for(int i = 2; i < argc; i++) {
            //printf("%li\n", strlen(argv[i]));

            for(int j = 0; j < strlen(argv[i]); j++){
                //printf("%d = %d\n", utmp_record.ut_user[j], argv[i][j]);

                if(utmp_record.ut_user[j] != argv[i][j]) {
                    string_equal_flag = 0;
                    //printf("\n");
                    break;
                }
            }
            if(string_equal_flag) {
                printf("Found user: \"%-8.8s\" in utmp file\n", utmp_record.ut_user);
                show_utmp_record(&utmp_record); 
            }
        }
        

    }


    close(file_descriptor);
    
}

void show_utmp_record(struct utmp* record) {
    printf("%-8.8s", record->ut_user);
    //printf("%-12.12s ", record->ut_line);
    //printf("%6d ", record->ut_pid);
    printf("%4d %-12.12s ", record->ut_type, get_utmp_type_name(record->ut_type));
    //printf("%12d ", record->ut_time);
    //printf("%s ", record->ut_host);
    putchar('\n');
}

char *utmp_type_names[] = {"EMPTY", "RUN_LVL", "BOOT_TIME", "OLD_TIME",
                            "NEW_TIME", "INIT_PROCESS", "LOGIN_PROCESS",
                            "USER_PROCESS", "DEAD_PROCESS", "ACCOUNTING"};


char *get_utmp_type_name(int type_number) {
    return utmp_type_names[type_number];
}

