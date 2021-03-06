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
#define WAITING_PERIOD 2500


typedef struct PlayerPackage
{
    int score;
    enum GameStatus status;
    struct Cell game_field[GAME_FIELD_HEIGHT * GAME_FIELD_WIDTH];
} PlayerPackage;

void *runServer(Game *game);
void *runClient(Game *game);

#endif