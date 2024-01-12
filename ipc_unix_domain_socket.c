#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/uds_socket"

void child_process() {
    char buffer[128];
    struct sockaddr_un addr;
    int fd;

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path)-1);

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect error");
        exit(-1);
    }

    read(fd, buffer, sizeof(buffer));
    printf("Received: %s\n", buffer);

    close(fd);
}

void parent_process() {
    struct sockaddr_un addr;
    char buffer[] = "Hello from parent!";
    int fd, cl;

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path)-1);

    unlink(SOCKET_PATH);

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind error");
        exit(-1);
    }

    if (listen(fd, 5) == -1) {
        perror("listen error");
        exit(-1);
    }

    if ((cl = accept(fd, NULL, NULL)) == -1) {
        perror("accept error");
        exit(-1);
    }

    write(cl, buffer, sizeof(buffer));
    close(cl);
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        // Error occurred while forking
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        sleep(10); // Ensure that the server is set up
        child_process();
    } else {
        // Parent process
        parent_process();
    }

    return 0;
}