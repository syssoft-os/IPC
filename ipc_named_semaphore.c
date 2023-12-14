#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

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

void recv_semaphore (char *semaphore_name) {
    sem_t *semaphore = sem_open(semaphore_name, O_CREAT|O_EXCL, 0644, 0);
    if (semaphore == SEM_FAILED) {
        perror("Error initializing semaphore\n");
        exit(1);
    }
    printf("recv: Wait on semaphore %s\n",semaphore_name);
    sem_wait(semaphore);
    printf("recv: wait finished\n");
    int result = sem_unlink(semaphore_name);
    if (result == -1) {
        perror("Error unlinking semaphore");
    }

}

void send_semaphore (char *semaphore_name) {
    sem_t *semaphore = sem_open(semaphore_name, O_CREAT, 0644, 0);
    if (semaphore == SEM_FAILED) {
        perror("Error initializing semaphore\n");
        exit(1);
    }
    printf("send: Will post on semaphore %s\n",semaphore_name);
    sem_post(semaphore);
    printf("send: post finished\n");
    int result = sem_close(semaphore);
    if (result == -1) {
        perror("Error closing semaphore");
    }

}

int main(int ac, char **av) {
    if (ac == 1) {
        char *semaphoreName = addPidSuffix("/ipc_named_semaphore");
        recv_semaphore(semaphoreName);
    }
    else if (ac == 2) {
        send_semaphore(av[1]);
    }
}
