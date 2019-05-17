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

typedef struct PlayerPackage
{
    int score;
    struct Cell game_field[GAME_FIELD_HEIGHT * GAME_FIELD_WIDTH];
    enum GameStatus status;
} PlayerPackage;

void runServer(Game *game);
void runClient(Game *game);

#endif