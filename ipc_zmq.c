#include <zmq.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_SOCKET_ADDR "ipc:///tmp/zmqtest"

int main(int argc, char *argv[]) {
    void *context = zmq_ctx_new();
    assert(context);

    if (argc == 1) {
        // No argument given, create a PAIR socket
        void *socket = zmq_socket(context, ZMQ_PAIR);
        assert(socket);
        int rc = zmq_bind(socket, DEFAULT_SOCKET_ADDR);
        assert(rc == 0);
        printf("Created a ZMQ queue at: %s\n", DEFAULT_SOCKET_ADDR);

        char buffer[100];
        zmq_recv(socket, buffer, 100, 0);
        printf("Received: %s\n", buffer);

        char reply[] = "World";
        zmq_send(socket, reply, strlen(reply)+1, 0);
        printf("Sent: %s\n",reply);
        zmq_close(socket);
    } else {
        // Argument given, create a PAIR socket and connect to the given address
        void *socket = zmq_socket(context, ZMQ_PAIR);
        assert(socket);
        int rc = zmq_connect(socket, argv[1]);
        assert(rc == 0);
        printf("Connected to a ZMQ queue at: %s\n", argv[1]);

        char buffer[] = "Hello";
        zmq_send(socket, buffer, strlen(buffer)+1, 0);
        printf("Sent: %s\n",buffer);

        char reply[100];
        zmq_recv(socket, reply, 100, 0);
        printf("Received: %s\n", reply);
        zmq_close(socket);
    }

    zmq_ctx_destroy(context);
    return 0;
}