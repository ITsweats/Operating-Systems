#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

int alarms = 0;
time_t start_time;

void alarm_handler(int signum) {
    alarms++;
    alarm(1);
}

void sigint_handler(int signum) {
    time_t end_time = time(NULL);
    int total_time = (int)(end_time - start_time);
    printf("\nExecution time: %d seconds\nNumber of alarms: %d\n", total_time, alarms);
    exit(0);
}

int main(int argc, char *argv[]) {
    start_time = time(NULL);

    signal(SIGALRM, alarm_handler);
    signal(SIGINT, sigint_handler);

    alarm(1);

    while (1);

    return 0;
}
