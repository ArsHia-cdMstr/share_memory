#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

int main() {
    int shmid;
    key_t key = 1234;
    int *shared_data;

    // Create a shared memory segment
    shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach the shared memory segment to the address space of the parent and child processes
    shared_data = (int*) shmat(shmid, NULL, 0);
    if (shared_data == (int*) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        int num_to_send = 42; // The integer to send to the parent process

        *shared_data = num_to_send; // Write the integer to the shared memory segment

        exit(EXIT_SUCCESS);
    } else { // Parent process
        int status;

        // Wait for the child process to write to the shared memory segment
        waitpid(pid, &status, 0);

        int num_from_child = *shared_data; // Read the integer from the shared memory segment

        printf("Received number %d from child process\n", num_from_child);

        // Detach the shared memory segment from the address space of the parent and child processes
        shmdt(shared_data);

        // Delete the shared memory segment
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}