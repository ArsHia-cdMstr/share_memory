#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid;
    int num_from_child;

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        int num_to_send = 42; // The integer to send to the parent process

        close(pipefd[0]); // Close the unused read end of the pipe
        write(pipefd[1], &num_to_send, sizeof(int)); // Write the integer to the pipe
        close(pipefd[1]); // Close the write end of the pipe

        exit(EXIT_SUCCESS);
    } else { // Parent process
        int status;

        close(pipefd[1]); // Close the unused write end of the pipe
        read(pipefd[0], &num_from_child, sizeof(int)); // Read the integer from the pipe
        close(pipefd[0]); // Close the read end of the pipe

        printf("Received number %d from child process\n", num_from_child);

        // Wait for the child process to terminate
        waitpid(pid, &status, 0);
    }

    return 0;
}