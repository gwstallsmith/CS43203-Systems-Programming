#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void pipeline(char *programs[]) {
    int file_descriptors[2];
    pid_t pid;
    int i = 0;

    // fd[0] is read end
    // fd[1] is write endexit(EXIT_FAILURE);

    while(programs[i] != NULL) {
        pipe(file_descriptors);

        if((pid = fork()) == -1) {
            perror("fork");
            return;

        } else if (pid == 0) {
            close(file_descriptors[1]);
            dup2(file_descriptors[0], STDIN_FILENO);
            close(file_descriptors[0]);

            // Execute pipe
            execlp(programs[i], programs[i + 1], NULL);

        } else if(pid > 0) {
            close(file_descriptors[0]);
            dup2(file_descriptors[1], STDOUT_FILENO);
            close(file_descriptors[1]);
        }
        i++;
    }
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Usage: %s <program1> <program2> ...\n", argv[0]);
        return -1;
    }

    pipeline(&argv[1]);

    return 0;
}