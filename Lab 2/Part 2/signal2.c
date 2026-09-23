#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int signaled = 0;

void handler(int signum) {
    printf("Hello World!\n");
    signaled = 1;
    alarm(5);
}

int main(int argc, char *argv[]) {
    signal(SIGALRM, handler);
    alarm(5);

    while (1) {
        while (!signaled);
        printf("Turing was right!\n");
        signaled = 0;
    }

    return 0;
}