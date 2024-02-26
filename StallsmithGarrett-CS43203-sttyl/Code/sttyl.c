#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <stdbool.h>

void print_usage() {
    printf("Usage: ./sttyl [OPTION]...\n");
    printf("\nOptions:\n");
    printf("  -a, --all           print all current settings\n");
    printf("  -e, --erase=CHAR    set erase CHAR (default is 'erase')\n");
    printf("  -icrnl              turn on icrnl\n");
    printf("  -onlcr              turn on onlcr\n");
    printf("  -echo               turn on echo\n");
    printf("  -echoe              turn on echoe\n");
    printf("  -olcuc              turn on olcuc\n");
    printf("  -tabs               turn on tabs\n");
    printf("  -icanon             turn on icanon\n");
    printf("  -isig               turn on isig\n");
    printf("  -h, --help          display this help and exit\n");
}

void print_version() {
    printf("mystty version 1.0\n");
}

void print_settings(struct termios *term) {
    printf("Speed: %d\n", (int)cfgetispeed(term));
    printf("Erase character: %c\n", term->c_cc[VERASE]);
    printf("icrnl: %s\n", (term->c_iflag & ICRNL) ? "on" : "off");
    printf("onlcr: %s\n", (term->c_oflag & ONLCR) ? "on" : "off");
    printf("echo: %s\n", (term->c_lflag & ECHO) ? "on" : "off");
    printf("echoe: %s\n", (term->c_lflag & ECHOE) ? "on" : "off");
    printf("olcuc: %s\n", (term->c_oflag & OLCUC) ? "on" : "off");
    printf("icanon: %s\n", (term->c_lflag & ICANON) ? "on" : "off");
    printf("isig: %s\n", (term->c_lflag & ISIG) ? "on" : "off");
}

int main(int argc, char *argv[]) {
    struct termios term;
    char *erase_char = NULL;

    if(argc) {
        print_settings(&term);
    }

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--all") == 0 || argc == 1) {
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
            bool enable = true;
            char *option = argv[i];

            if (option[0] == '-') {
                enable = false;
                option++;
            }

            if (strcmp(option, "icrnl") == 0) {
                if (enable)
                    term.c_iflag |= ICRNL;
                else
                    term.c_iflag &= ~ICRNL;
            } else if (strcmp(option, "onlcr") == 0) {
                if (enable)
                    term.c_oflag |= ONLCR;
                else
                    term.c_oflag &= ~ONLCR;
            } else if (strcmp(option, "echo") == 0) {
                if (enable)
                    term.c_lflag |= ECHO;
                else
                    term.c_lflag &= ~ECHO;
            } else if (strcmp(option, "echoe") == 0) {
                if (enable)
                    term.c_lflag |= ECHOE;
                else
                    term.c_lflag &= ~ECHOE;
            } else if (strcmp(option, "olcuc") == 0) {
                if (enable)
                    term.c_oflag |= OLCUC;
                else
                    term.c_oflag &= ~OLCUC;
            } else if (strcmp(option, "icanon") == 0) {
                if (enable)
                    term.c_lflag |= ICANON;
                else
                    term.c_lflag &= ~ICANON;
            } else if (strcmp(option, "isig") == 0) {
                if (enable)
                    term.c_lflag |= ISIG;
                else
                    term.c_lflag &= ~ISIG;
            } else {
                fprintf(stderr, "Invalid option: %s\n", argv[i]);
                print_usage();
                return 1;
            }
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
