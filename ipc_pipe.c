#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void recv_pipe ( int fd ) {
    char buffer[100];
    ssize_t numBytes = read(fd, buffer, sizeof(buffer));
    if (numBytes == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    printf("Received: %s\n", buffer);
}

void send_pipe ( int fd ) {
    const char* message = "Ping";
    printf("About to send: %s\n", message);
    ssize_t numBytes = write(fd, message, sizeof(message));
    if (numBytes == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
}

int main() {
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Child process
        close(pipefd[1]);  // Close unused write end
        recv_pipe(pipefd[0]);
        close(pipefd[0]);
    } else {  // Parent process
        close(pipefd[0]);  // Close unused read end
        sleep(2);
        send_pipe(pipefd[1]);
        close(pipefd[1]);
    }

    return 0;
}
