#ifndef MZAPO_TEMPLATE_SERVER_H
#define MZAPO_TEMPLATE_SERVER_H

#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include "../Game/Game.h"
#include "../Player/Player.h"

#define TARGET_PORT 5678

typedef struct Package
{
    int type;
    // 0- player
    //1-game_field
    void *data;
} Package;

typedef struct PlayerPackage
{
    int type;
    // 0- player
    //1-game_field
    int score;
    char ip[16];
    enum GameStatus status;
    int level;
} PlayerPackage;

void runServer(Game *game);
void runClient(Game *game);

#endif