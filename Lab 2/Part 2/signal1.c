#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int signaled = 0;

void handler(int signum) {
    printf("Hello World!\n");
    signaled = 1;
}

int main(int argc, char *argv[]) {
    signal(SIGALRM, handler);
    alarm(5);

    while (!signaled);

    printf("Turing was right!\n");
    return 0;
}
