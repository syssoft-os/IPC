#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>


bool signal_received = false;

void sigusr1_handler(int signum) {
    signal_received = true;
}

void child_process() {
    printf("Hello from the child process!\n");
    signal(SIGUSR1, sigusr1_handler);
    while (true) {
        if (signal_received) {
            printf("Received SIGUSR1 signal\n");
            signal_received = false;
        }
        else
            printf("Waiting for SIGUSR1 signal\n");
        sleep(1);
    }
}

void parent_process(int child_pid) {
    printf("Hello from the parent process!\n");
    printf("Child process has PID: %d\n", child_pid);
    sleep(10);
    printf("Sending SIGUSR1 signal to child process\n");
    kill(child_pid, SIGUSR1);
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        // Error occurred while forking
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        child_process();
    } else {
        // Parent process
        parent_process(pid);
    }

    return 0;
}