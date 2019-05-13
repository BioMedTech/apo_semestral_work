#include "server.h"

void runServer(Game *game){
     int _socket, nBytes;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t serverAddrSize, clientAddrSize;
    struct ifreq ifr;

    /*Create UDP socket*/
    _socket = socket(PF_INET, SOCK_DGRAM, 0);

    ifr.ifr_addr.sa_family = AF_INET;
    /*Configure settings in address struct*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(TARGET_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
    ioctl(_socket, SIOCGIFADDR, &ifr);

    printf("My ip: %s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    /*Bind socket with address struct*/
    if (bind(_socket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == SO_ERROR) {
        return 1;
    }

    /*Initialize size variable to be used later on*/
    serverAddrSize = sizeof serverAddr;
    clientAddrSize = sizeof clientAddr;

    int _continue = 1;
   
    printf("Server is listening on port %d\n", TARGET_PORT);
    while (_continue) {
        if (nBytes = recvfrom(_socket, game->opponent, sizeof(Player), 0, (struct sockaddr *) &clientAddr,
                                &clientAddrSize) == SO_ERROR) {
            fprintf(stderr, "Socket error!\n");
            getchar();
            return 1;
        }
        char *ip = inet_ntoa(clientAddr.sin_addr);
     
        if (game->opponent->status==GAME_INIT && !strcmp(game->currentPlayer->ip, ip)){
            _continue = 0;
            strcpy(game->opponent->ip, ip);
        }
    }

    int gameContinue = 1;
    game->currentPlayer->status == GAME_INIT;

    while (gameContinue){
        if (nBytes = recvfrom(_socket, game->opponent, sizeof(Player), 0, (struct sockaddr *) &clientAddr,
                                &clientAddrSize) == SO_ERROR) {
            fprintf(stderr, "Socket error!\n");
            getchar();
            return 1;
        }

        gameContinue = game->opponent->status != GAME_END;
    }
}



void runClient(Game *game){
    ssize_t n;
    struct sockaddr_in broadcast;
    int _socketClient = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&broadcast, 0, sizeof(struct sockaddr_in));

    int yes = 1;
    if (setsockopt(_socketClient, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1 || 
    setsockopt(_socketClient, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes)) == -1) {
        perror("cannot set socket options");
        exit(1);
    }

    // setup broadcast 
    broadcast.sin_family = AF_INET;
    broadcast.sin_port = htons(TARGET_PORT);
    // broadcast.sin_addr.s_addr = INADDR_BROADCAST;
    // broadcast.sin_addr.s_addr = inet_addr("147.32.216.255");

    int once = 1;
    char ipbuf[16];

    unsigned int len = sizeof(broadcast);
    int a=0;
    
    do {
       snprintf(ipbuf, sizeof ipbuf, "192.168.202.%d", a);
       printf("Sending to: %s\n", ipbuf);
       broadcast.sin_addr.s_addr = inet_addr(ipbuf);

       if (sendto(_socketClient, game->currentPlayer, sizeof(Player), 0, (const struct sockaddr *)&broadcast, len) < 0)
           printf("Error sending update.\n");  

        a++;
    } while (!game->opponent->ip);

    do {
        if (sendto(_socketClient, game->currentPlayer, sizeof(Player), 0, (const struct sockaddr *)&broadcast, len) < 0)
            printf("Error sending update.\n");   

        sleep(3);

    } while(game->currentPlayer->status != GAME_END);

    printf("Ending update sender...\n");
    return NULL;
}

// Send, get: current game object 