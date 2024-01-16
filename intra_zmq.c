#include <zmq.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_SOCKET_ADDR "inproc://zmqtest"

void *worker_routine(void *context) {
    void *socket = zmq_socket(context, ZMQ_PAIR);
    assert(socket);
    int rc = zmq_connect(socket, DEFAULT_SOCKET_ADDR);
    assert(rc == 0);

    char buffer[] = "Hello";
    zmq_send(socket, buffer, strlen(buffer)+1, 0);

    char reply[10];
    zmq_recv(socket, reply, 10, 0);
    printf("Received: %s\n", reply);

    zmq_close(socket);
    return NULL;
}

int main() {
    void *context = zmq_ctx_new();
    assert(context);

    void *socket = zmq_socket(context, ZMQ_PAIR);
    assert(socket);
    int rc = zmq_bind(socket, DEFAULT_SOCKET_ADDR);
    assert(rc == 0);

    pthread_t worker;
    pthread_create(&worker, NULL, worker_routine, context);

    char buffer[10];
    zmq_recv(socket, buffer, 10, 0);
    printf("Received: %s\n", buffer);

    char reply[] = "World";
    zmq_send(socket, reply, strlen(reply)+1, 0);

    pthread_join(worker, NULL);

    zmq_close(socket);
    zmq_ctx_destroy(context);
    return 0;
}