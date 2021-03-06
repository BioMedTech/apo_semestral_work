#include "server.h"
int found_ip=0;


void *runServer(Game *game) {
    int _socket, nBytes;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t  clientAddrSize;
    struct ifreq ifr;

    /*Create UDP socket*/
    _socket = socket(PF_INET, SOCK_DGRAM, 0);

    ifr.ifr_addr.sa_family = AF_INET;
    /*Configure settings in address struct*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(TARGET_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);
    ioctl(_socket, SIOCGIFADDR, &ifr);

    strncpy(game->currentPlayer->ip, inet_ntoa(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr), 15);

    /*Bind socket with address struct*/
    if (bind(_socket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == SO_ERROR) {
        perror("Bind: ");
        return 1;
    }

    /*Initialize size variable to be used later on*/
    clientAddrSize = sizeof clientAddr;

    int _continue = 1;
    PlayerPackage *package=(PlayerPackage*) calloc (1, sizeof(PlayerPackage));

    printf("Server is listening on port %d\n", TARGET_PORT);
    while (_continue) {
        if ((nBytes = recvfrom(_socket, package, sizeof(PlayerPackage), 0, (const struct sockaddr *)&clientAddr,
                          &clientAddrSize)) < 0)
            perror("recvfrom: 43");

        char *ip = inet_ntoa(clientAddr.sin_addr);

        if (strncmp(game->currentPlayer->ip, ip, 15) != 0) {
            _continue = 0;
            strncpy(game->opponent->ip, ip, 15);
            found_ip = 1;
        }
    }

    int gameContinue = 1;
    game->currentPlayer->status == GAME_INIT;

    while (gameContinue) {
        
        if ((nBytes = recvfrom(_socket, package, sizeof(PlayerPackage), 0, &clientAddr,
                              &clientAddrSize)) < 0) {
            perror("Socket error");
            return 1;
        }
        
        if (!strncmp(game->opponent->ip, inet_ntoa(clientAddr.sin_addr), 15)){
            memcpy(game->opponent->game_field, &package->game_field, GAME_FIELD_HEIGHT * GAME_FIELD_WIDTH * sizeof(Cell));
            game->opponent->score = package->score;
            game->opponent->status = package->status;
        }
       
        if (game->opponent->status == GAME_END)
        {
            clientAddr.sin_addr.s_addr = inet_addr(game->opponent->ip);
            gameContinue=0;
            package->status = GAME_END;
            if (sendto(_socket, package, sizeof(PlayerPackage), 0, (const struct sockaddr *)&clientAddr, clientAddrSize) < 0) {
                perror("sendto: 76");
            }
        }
    }
    
    free(game->opponent->game_field);
    game->opponent->game_field = NULL;
    
    free(package);
    printf("Ending recieving updates...\n");

    return NULL;
}


void *runClient(Game *game) {
    ssize_t n;
    struct sockaddr_in broadcast;
    int _socketClient = socket(AF_INET, SOCK_DGRAM, 0);
    socklen_t len = sizeof(broadcast);

    int yes = 1;
    if (setsockopt(_socketClient, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1 ||
        setsockopt(_socketClient, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes)) == -1
            ) {
        perror("cannot set socket options");
        exit(1);
    }

    // setup broadcast 
    memset(&broadcast, 0, sizeof(broadcast));
    broadcast.sin_family = AF_INET;
    broadcast.sin_port = htons(TARGET_PORT);

    char ipbuf[16];
    int a = 0;

    PlayerPackage *package = calloc(1, sizeof(PlayerPackage));
    
    do {
        snprintf(ipbuf, sizeof ipbuf, "192.168.202.%d", a);
        broadcast.sin_addr.s_addr = inet_addr(ipbuf);
        if (sendto(_socketClient, package, sizeof(PlayerPackage), 0, (const struct sockaddr *) &broadcast, len) < 0)
            perror("Error sending update. ");

        a = (a + 1) % 256;
        sleep(0.3);
    } while (!found_ip);

    game->currentPlayer->status = GAME_IN_PROGRESS;
    broadcast.sin_addr.s_addr = inet_addr(game->opponent->ip);

    do {
        package->score = game->currentPlayer->score,
        package->status = game->currentPlayer->status;
        memcpy(&package->game_field, game->currentPlayer->game_field, GAME_FIELD_HEIGHT*GAME_FIELD_WIDTH * sizeof(Cell));

        if (sendto(_socketClient, package, sizeof(PlayerPackage), 0, (const struct sockaddr *)&broadcast, len) < 0)
            perror("Error sending update");

        sleep(0.3);
    } while (game->currentPlayer->status != GAME_END);

    int _continue=1;
    
    while (_continue)
    {
        package->status = GAME_END;
        if (sendto(_socketClient, package, sizeof(PlayerPackage), 0, (const struct sockaddr *)&broadcast, len) < 0)
            perror("sendto: 148");
        if (recvfrom(_socketClient, package, sizeof(PlayerPackage), 0, NULL, NULL) < 0)
            perror("recvfrom: 151");

        _continue = package->status != GAME_END;
    }
    

    free(package);
    
    printf("Ending update sender...\n");
    return NULL;
}
