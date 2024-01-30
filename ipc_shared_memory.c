#include <stdio.h>

int main (int ac, char **av) {
    printf("Stop! In most cases, writing your own shared memory solution is a bad idea.\n");
    printf("Use a library instead. For example, Boost.Interprocess.\n");
    printf("Or rely on message-based approaches such as UNIX sockets, ZeroMQ, or TCP/IP.\n");
}