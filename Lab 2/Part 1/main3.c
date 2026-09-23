#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void child_process() {
    // Seed using time combined with PID so both children get different random sequences
    srandom(time(NULL) ^ getpid());

    // Random number of iterations: up to 30 (e.g., 1 to 30)
    int iterations = (random() % 30) + 1;

    for (int i = 0; i < iterations; i++) {
        printf("Child Pid: %d is going to sleep!\n", getpid());

        // Sleep for a random amount of time: up to 10 seconds (e.g., 1 to 10)
        int sleep_time = (random() % 10) + 1;
        sleep(sleep_time);

        printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n", getpid(), getppid());
    }

    exit(0);
}

int main() {
    pid_t pid1, pid2;

    // Fork the first child
    pid1 = fork();
    if (pid1 < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid1 == 0) {
        child_process();
    }

    // Fork the second child (only reached by the parent)
    pid2 = fork();
    if (pid2 < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid2 == 0) {
        child_process();
    }

    // Parent waits for both child processes
    int status;
    for (int i = 0; i < 2; i++) {
        pid_t completed_pid = wait(&status);
        printf("Child Pid: %d has completed\n", completed_pid);
    }

    return 0;
}