#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

char* addPidSuffix(const char* str) {
    pid_t pid = getpid();
    int strLength = strlen(str);
    int pidLength = snprintf(NULL, 0, "_%d", pid);

    char* result = malloc(strLength + pidLength + 1);
    if (result == NULL) {
        perror("Error allocating memory");
        exit(1);
    }
    strcpy(result, str);
    sprintf(result + strLength, "_%d", pid);

    return result;
}

void recv_pipe(char *pipe_name) {
    int pipe_fd = open(pipe_name, O_RDONLY);
    if (pipe_fd == -1) {
        perror("Failed to open named pipe");
        return;
    }
    char buffer[1024];
    ssize_t bytes_read = read(pipe_fd, buffer, sizeof(buffer));
    if (bytes_read == -1) {
        perror("Failed to read from named pipe");
        close(pipe_fd);
        return;
    }
    printf("Received: %s\n", buffer);
    close(pipe_fd);
}

void send_pipe(char *pipe_name) {
    int pipe_fd = open(pipe_name, O_WRONLY);
    if (pipe_fd == -1) {
        perror("Failed to open named pipe");
        return;
    }
    char buffer[1024];
    sprintf(buffer, "Hello from %d", getpid());
    ssize_t bytes_written = write(pipe_fd, buffer, strlen(buffer)+1); // Send the null terminator as well
    if (bytes_written == -1) {
        perror("Failed to write to named pipe");
        close(pipe_fd);
        return;
    }
    close(pipe_fd);
}

int main ( int ac, char **av ) {
    if (ac ==  1) {
        char* pipe_name = addPidSuffix("named_pipe");
        if (mkfifo(pipe_name, 0666) == -1) {
            perror("Failed to create named pipe");
            exit(-1);
        }
        printf("Named pipe is <%s>\n", pipe_name);
        recv_pipe(pipe_name);
        unlink(pipe_name);
        free(pipe_name);
    }
    else if (ac == 2) {
        send_pipe(av[1]);
    }
    else {
        printf("Usage: <%s> for creating a named pipe and receive or <%s pipe_name> for sending\n", av[0], av[0]);
        exit(-1);
    }
}