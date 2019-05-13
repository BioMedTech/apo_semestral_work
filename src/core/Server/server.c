#include "server.h"

void runServer(Game *currentGame){
     int _socket, nBytes;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t serverAddrSize, clientAddrSize;

    /*Create UDP socket*/
    _socket = socket(PF_INET, SOCK_DGRAM, 0);

    /*Configure settings in address struct*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(TARGET_PORT);
   
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /*Bind socket with address struct*/
    if (bind(_socket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == SO_ERROR) {
        if (VERBOSE) printf("Bind error!");
        return 1;
    }

    setTimeout(_socket);

    /*Initialize size variable to be used later on*/
    serverAddrSize = sizeof serverAddr;
    clientAddrSize = sizeof clientAddr;

    Game *game = (Game *)calloc(1, sizeof(Game));

    int _continue = 1;
   
    while (_continue) {
        if (nBytes = recvfrom(_socket, game->opponent, sizeof(Player), 0, (struct sockaddr *) &clientAddr,
                                &clientAddrSize) == SO_ERROR) {
            fprintf(stderr, "Socket error!\n");
            getchar();
            return 1;
        }
        if (game->opponent->status==GAME_INIT && game->opponent.id!=game->currentPlayer.id){
            _continue = 0;
            // sendto(_socket, currentGame->currentPlayer, sizeof(game), 0, (struct sockaddr *) &clientAddr, clientAddrSize);
            game->opponent->ip = inet_ntoa(clientAddr.sin_addr.s_addr);
        }
    }

    int gameContinue = 1;
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
    s   etsockopt(_socketClient, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes)) == -1) {
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
    do {
        for (int a = 0; !game->opponent->ip && a < 255; ++a) {
            snprintf(ipbuf, sizeof ipbuf, "147.32.216.%d", a);
            printf("Sending to: %s\n", ipbuf);
            broadcast.sin_addr.s_addr = inet_addr(ipbuf);
    
            if (sendto(_socketClient, game->currentPlayer, sizeof(Player), 0, (const struct sockaddr *)&broadcast, len) < 0)
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

// Send, get: current game object 