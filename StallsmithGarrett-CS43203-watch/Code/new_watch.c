#include	<stdio.h>
#include	<stdlib.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<utmp.h>
#include	<unistd.h>
#include	<string.h>

#define MAX_USERS 100

// Structure to store user login information
struct UserLogin {
    char username[UT_NAMESIZE];
    int logged_in;
};

// Function prototypes
void dump_utmp_file(char* filename, int argc, char* argv[]);
void show_utmp_record(struct utmp* record);
char *get_utmp_type_name(int type_number);
void assign_snapshot(char *filename, struct utmp* snapshot);
int find_user(struct UserLogin users[], int count, char *username);

int main(int argc, char* argv[]) {
    int sleep_timer = atoi(argv[1]);

    if (atoi(argv[1]) <= 0) {
        sleep_timer = 300;
    }
    if (argc < 2) {
        perror("Provide time per scan and user names you would like to scan");
        return 1;
    }
    if (argc < 3) {
        perror("Provide user names you would like to scan");
        return 1;
    }

    printf("Scanning for:\n");

    for (int i = 1; i < argc; i++) {
        if (i == 1) {
            printf("%i seconds\n\nFor Users:\n", sleep_timer);
        } else {
            printf("%s\n", argv[i]);
        }
        if (i == argc - 1) {
            printf("\n");
        }
    }

    while (1) {
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

    struct UserLogin users[MAX_USERS];
    memset(users, 0, sizeof(users));

    int num_users = 0;

    while (read(file_descriptor, &utmp_record, sizeof(utmp_record)) == sizeof(utmp_record)) {
        for (int i = 2; i < argc; i++) {
            if (strncmp(utmp_record.ut_user, argv[i], UT_NAMESIZE) == 0) {
                int user_index = find_user(users, num_users, utmp_record.ut_user);
                if (user_index == -1) {
                    // New user found
                    if (num_users < MAX_USERS) {
                        strncpy(users[num_users].username, utmp_record.ut_user, UT_NAMESIZE);
                        users[num_users].logged_in = 1;
                        num_users++;
                        printf("User \"%s\" is logged in.\n", utmp_record.ut_user);
                    }
                } else {
                    // User already exists, check if they've logged out
                    if (utmp_record.ut_type == DEAD_PROCESS) {
                        users[user_index].logged_in = 0;
                        printf("User \"%s\" logged out.\n", utmp_record.ut_user);
                    }
                }
            }
        }
    }

    close(file_descriptor);
}

int find_user(struct UserLogin users[], int count, char *username) {
    for (int i = 0; i < count; i++) {
        if (strncmp(users[i].username, username, UT_NAMESIZE) == 0) {
            return i;
        }
    }
    return -1;
}

void show_utmp_record(struct utmp* record) {
    printf("%-8.8s", record->ut_user);
    printf("%4d %-12.12s ", record->ut_type, get_utmp_type_name(record->ut_type));
    putchar('\n');
}

char *utmp_type_names[] = {"EMPTY", "RUN_LVL", "BOOT_TIME", "OLD_TIME",
                            "NEW_TIME", "INIT_PROCESS", "LOGIN_PROCESS",
                            "USER_PROCESS", "DEAD_PROCESS", "ACCOUNTING"};

char *get_utmp_type_name(int type_number) {
    return utmp_type_names[type_number];
}
