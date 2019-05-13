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

#define TARGET_PORT 3333
char * opponent;

void server(){
    
    int nBytes;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t serverAddrSize, clientAddrSize;

     int _socket = socket(AF_INET, SOCK_DGRAM, 0);

    int enabled = 1;
    setsockopt(_socket, SOL_SOCKET, SO_BROADCAST, &enabled, sizeof(enabled));
      int yes=1;

    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &yes,
                    sizeof(yes)) == -1) {
        perror("setsockopt (SO_REUSEADDR)");
        exit(1);
    }

    char loopch=0;

    if (setsockopt(_socket, IPPROTO_IP, IP_MULTICAST_LOOP,
                (char *)&loopch, sizeof(loopch)) < 0) {
    perror("setting IP_MULTICAST_LOOP:");
    exit(1);
    }

    /*Configure settings in address struct*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(TARGET_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
   
    /*Bind socket with address struct*/
    if (bind(_socket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == SO_ERROR) {
        return 1;
    }

    /*Initialize size variable to be used later on*/
    serverAddrSize = sizeof serverAddr;
    clientAddrSize = sizeof clientAddr;

    char *str = calloc (1024, sizeof(char));
    // listen(_socket, 5);

    socklen_t clilen = sizeof(clientAddr);
    printf("listening on port 3333.\n");
    
    int once=1;

    while (1){    
            if (recvfrom(_socket, str, 1024, 0, (struct sockaddr *) &clientAddr, &clientAddrSize) < 0){
                printf("Recieving error\n");
            }

        if(opponent && once){
            opponent = inet_ntoa(clientAddr.sin_addr.s_addr);
            once=0;
        }
            printf("%s\n", str);
    }
    return NULL;
}

void client(){
    ssize_t n;
    struct sockaddr_in broadcast;
    int _socketClient = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&broadcast, 0, sizeof(struct sockaddr_in));

    // buffer
    char* buffer = "Hello from Nastya";
    int broadcastEnable=1;
    setsockopt(_socketClient, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));


    int yes = 1;
    if (setsockopt(_socketClient, SOL_SOCKET, SO_REUSEADDR, &yes,
                    sizeof(yes)) == -1) {
        perror("setsockopt (SO_REUSEADDR)");
        exit(1);
    }

    // setup broadcast 
    broadcast.sin_family = AF_INET;
    broadcast.sin_port = htons(TARGET_PORT);
    // broadcast.sin_addr.s_addr = INADDR_BROADCAST;
    broadcast.sin_addr.s_addr = inet_addr("147.32.216.255");

    char *answer=(char *)calloc(1024, sizeof(char));
    int once = 1;
    // int i = 0;
    char ipbuf[256];

    unsigned int len = sizeof(broadcast);
    do {
        for (int a = 0; a < 255; ++a) {
                snprintf(ipbuf, sizeof ipbuf, "147.32.216.%d", a);
                printf("Sending to: %s\n", ipbuf);
                broadcast.sin_addr.s_addr = inet_addr(ipbuf);
                
                if (sendto(_socketClient, buffer, 1024, 0, (const struct sockaddr *)&broadcast, len) < 0)
                    printf("Error sending update.\n");
                
        }

        sleep(3);
      
        // if (opponent && once){
        //    printf("%s\n", opponent);
        //    broadcast.sin_addr.s_addr =inet_addr(opponent);
        //    once = 0;
        // }
    } while(1);

    free(buffer);
    printf("Ending update sender...\n");
    return NULL;
}

pthread_t game_thread;
pthread_t server_thread;

int main(){

    pthread_create(&server_thread, NULL, server, NULL);
    sleep(1);
    pthread_create(&game_thread, NULL, client, NULL);
    pthread_join(server_thread, NULL);
    pthread_join(game_thread, NULL);
}
