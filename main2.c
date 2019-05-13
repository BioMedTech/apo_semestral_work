#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define TARGET_PORT 3334

int _socket;

void server()
{
    // init data
    struct sockaddr_in my_addr, cli_addr;
    char buf[1024];

    // enable broadcast
    int enabled = 1;
    setsockopt(_socket, SOL_SOCKET, SO_BROADCAST, &enabled, sizeof(enabled));

    // setup server
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(TARGET_PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    // bind socket
    if (bind(_socket, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0)
    {
        fprintf(stderr, "ERROR binding socket, exiting.\n");
        return NULL;
    }

    // listen
    // listen(_socket, 5);

    // socket length
    socklen_t clilen;
    clilen = sizeof(cli_addr);
    // main loop
    printf("listening on port %d.\n", TARGET_PORT);
    while (1)
    {
        // select
        struct timeval timeout = {3, 0};
        fd_set in_set;
        FD_ZERO(&in_set);
        FD_SET(_socket, &in_set);
        
        if (select(_socket + 1, &in_set, NULL, NULL, &timeout) == -1)
        {
            fprintf(stderr, "ERROR select error in listener.\n");
        }

        // if (FD_ISSET(_socket, &in_set))
        // {
            // receive datagram
            if (recvfrom(_socket, buf, 1024, 0, (struct sockaddr *)&cli_addr, &clilen) < 0)
                perror("recvfrom");

            printf("Revieved %s\n", buf);
        // }
        // else // timeout of receive
        //     break;
    }

    printf("Ending update listener...\n");
    return NULL;
}

void client()
{
    // enable broadcast
    int enabled = 1;
    setsockopt(_socket, SOL_SOCKET, SO_BROADCAST, &enabled, sizeof(enabled));

    ssize_t n;

    // buffer
    char *buffer = "www";

    // setup broadcast
    struct sockaddr_in broadcast;
    broadcast.sin_family = AF_INET;
    broadcast.sin_port = htons(TARGET_PORT);
    broadcast.sin_addr.s_addr = INADDR_BROADCAST;

    unsigned int len = sizeof(struct sockaddr_in);

    // send update every one second
    printf("Broadcasting status.\n");
    do
    {
        n = sendto(_socket, buffer, 1024, 0, (const struct sockaddr *)&broadcast, len);
        if (n < 0)
            perror("sendto");

        sleep(1);
    } while (1);

    free(buffer);
    printf("Ending update sender...\n");
    return NULL;
}

pthread_t game_thread;
pthread_t server_thread;

int main(){
     _socket = socket(AF_INET, SOCK_DGRAM, 0);

    pthread_create(&server_thread, NULL, server, NULL);
    sleep(1);
    pthread_create(&game_thread, NULL, client, NULL);
    pthread_join(server_thread, NULL);
    pthread_join(game_thread, NULL);
}
