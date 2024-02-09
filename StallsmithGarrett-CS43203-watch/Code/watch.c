/*
Usage:

$ gcc watch.c -o Watch
$ ./Watch <user1> <user2> <userN> <sleep_interval>

*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <utmp.h>
#include <string.h>
#include <time.h>


// Function to check if a user is in the list of watched users
int is_user_watched(char *user, char **watched_users, int num_users);

// Function to compare the current and previous user lists and print changes
void check_user_changes(char **prev_users, int prev_count, char **curr_users, int curr_count);


int main(int argc, char *argv[]) {
    if (argc < 2) {                                                                 // Check if the number of command-line arguments is less than 2
        fprintf(stderr, "Usage: %s <user1> <user2> ... [interval]\n", argv[0]);     // Print usage message to stderr
        return 1;                                                                   // Return 1 to indicate an error
    }

    char **watched_users = &argv[1];    // Initialize a pointer to the list of watched users
    int num_users = argc - 1;           // Calculate the number of users to watch
    int interval = 300;    // Initialize the interval to the default value

    // Check if an interval is specified
    if (atoi(argv[argc - 1]) > 0) {                                 // Check if an interval argument exists
        interval = atoi(argv[argc-1]);                   // Convert the interval argument to an integer


        if (interval <= 0) {                                    // Check if the interval is less than or equal to zero
            fprintf(stderr, "Invalid interval specified\n");    // Print an error message for an invalid interval
            return 1;                                           // Return 1 to indicate an error
        }
    }


    struct utmp *ut;   // Declare a pointer to the utmp structure

    while (1) {   // Infinite loop for continuous monitoring
        // Open utmp file
        setutent();    // Set the file position to the beginning of the utmp file

        // Count number of currently logged in users
        int num_curr_users = 0;                 // Initialize the count of currently logged in users
        while ((ut = getutent()) != NULL) {     // Iterate through each entry in the utmp file
            if (ut->ut_type == USER_PROCESS && is_user_watched(ut->ut_user, watched_users, num_users)) {
                // Check if the entry represents a user process and if it's in the watched list
                num_curr_users++;   // Increment the count of currently logged in users
            }
        }


        // Allocate memory for the current user list
        char **curr_users = malloc(num_curr_users * sizeof(char *));   // Allocate memory for the current user list
        int index = 0;   // Initialize the index for populating the current user list

        // Populate the current user list
        setutent();                             // Set the file position to the beginning of the utmp file
        while ((ut = getutent()) != NULL) {     // Iterate through each entry in the utmp file again
            if (ut->ut_type == USER_PROCESS && is_user_watched(ut->ut_user, watched_users, num_users)) {
                // Check if the entry represents a user process and if it's in the watched list
                curr_users[index++] = strndup(ut->ut_user, UT_NAMESIZE - 1);   // Copy the user name to the current user list
            }
        }

        // Compare current and previous user lists and print changes
        static char **prev_users = NULL;    // Declare a static pointer to the previous user list
        static int prev_count = 0;          // Declare a static variable to store the count of previous users

        if (prev_users != NULL) {           // Check if the previous user list is not NULL
            check_user_changes(prev_users, prev_count, curr_users, num_curr_users);   // Compare and print user changes
            free(prev_users);               // Free memory allocated for the previous user list
        }

        prev_users = curr_users;            // Update the previous user list with the current user list
        prev_count = num_curr_users;        // Update the count of previous users with the count of current users

        // Close utmp file
        endutent();         // Close the utmp file and release associated resources
        printf("...\n");
        // Sleep for the specified interval before checking again
        sleep(interval);    // Pause the program execution for the specified interval
    }

    return 0;   // Return 0 to indicate successful program execution
}


// Function to check if a user is in the list of watched users
int is_user_watched(char *user, char **watched_users, int num_users) {
    for (int i = 0; i < num_users; i++) {           // Loop through the watched users list
        if (strcmp(user, watched_users[i]) == 0) {  // Compare the current user with the watched users
            return 1;                               // Return 1 if the user is found in the list
        }
    }
    return 0;                                       // Return 0 if the user is not found in the list
}

// Function to compare the current and previous user lists and print changes
void check_user_changes(char **prev_users, int prev_count, char **curr_users, int curr_count) {
    for (int i = 0; i < prev_count; i++) {                                  // Loop through the previous user list
        if (!is_user_watched(prev_users[i], curr_users, curr_count)) {      // Check if the user logged out
            printf("%s logged out\n", prev_users[i]);                       // Print the user who logged out
        }
    }
    for (int i = 0; i < curr_count; i++) {                                  // Loop through the current user list
        if (!is_user_watched(curr_users[i], prev_users, prev_count)) {      // Check if the user logged in
            printf("%s logged in\n", curr_users[i]);                        // Print the user who logged in
        }
    }
}

