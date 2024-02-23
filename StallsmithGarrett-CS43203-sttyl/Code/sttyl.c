#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

void print_usage() {
    printf("Usage: mystty [OPTION]...\n");
    printf("Print or change terminal settings.\n");
    printf("\nOptions:\n");
    printf("  -a, --all           print all current settings\n");
    printf("  -e, --erase=CHAR    set erase CHAR (default is 'erase')\n");
    printf("  -h, --help          display this help and exit\n");
    printf("  -v, --version       output version information and exit\n");
}

void print_version() {
    printf("mystty version 1.0\n");
}

void print_settings(struct termios* term) {
    printf("Speed: %d\n", (int) cfgetispeed(term));
    printf("Erase character: %c\n", term->c_cc[VERASE]);
    // Add more settings if needed
}

int main(int argc, char *argv[]) {
    struct termios term;
    char *erase_char = NULL;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--all") == 0) {
            tcgetattr(STDIN_FILENO, &term);
            print_settings(&term);
            return 0;
        } else if (strncmp(argv[i], "-e", 2) == 0 || strncmp(argv[i], "--erase=", 8) == 0) {
            if (strlen(argv[i]) == 2) {
                if (i + 1 < argc) {
                    erase_char = argv[++i];
                } else {
                    fprintf(stderr, "Option %s requires an argument.\n", argv[i]);
                    return 1;
                }
            } else {
                erase_char = &argv[i][8];
            }
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage();
            return 0;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            print_version();
            return 0;
        } else {
            fprintf(stderr, "Invalid option: %s\n", argv[i]);
            print_usage();
            return 1;
        }
    }

    // Change terminal settings if erase character is specified
    if (erase_char != NULL) {
        if (strlen(erase_char) != 1) {
            fprintf(stderr, "Erase character must be a single character.\n");
            return 1;
        }
        if (tcgetattr(STDIN_FILENO, &term) == -1) {
            perror("tcgetattr");
            return 1;
        }
        term.c_cc[VERASE] = erase_char[0];
        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) == -1) {
            perror("tcsetattr");
            return 1;
        }
    }

    return 0;
}
