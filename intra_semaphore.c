#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaphore;

void* thread_main(void* arg) {
    printf("Additional pthread waiting on semaphore\n");
    sem_wait(&semaphore);
    printf("Pthread: Wait on semaphore is over\n");
    return NULL;
}
    
int main() {
    pthread_t thread;
    int result;

    // Initialize the semaphore to be used within process (first 0) and with value 0
    result = sem_init(&semaphore, 0, 0);
    if (result != 0) {
        perror("Error initializing semaphore\n");
        return 1;
    }

    // Create the additional thread
    result = pthread_create(&thread, NULL, thread_main, NULL);
    if (result != 0) {
        printf("Error creating thread\n");
        return 1;
    }

    // Code to be executed by the main thread
    printf("Main thread executing\n");
    sleep(2);
    printf("Main thread posting on semaphore\n");
    sem_post(&semaphore);

    // Wait for the additional thread to finish
    result = pthread_join(thread, NULL);
    if (result != 0) {
        printf("Error joining thread\n");
        return 1;
    }

    // Destroy the semaphore
    result = sem_destroy(&semaphore);
    if (result != 0) {
        printf("Error destroying semaphore\n");
        return 1;
    }

    return 0;
}
