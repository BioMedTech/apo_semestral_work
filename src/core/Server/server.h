#ifndef MZAPO_TEMPLATE_SERVER_H
#define MZAPO_TEMPLATE_SERVER_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <stdlib.h>
#include "../Game/Game.h"
#include "../Player/Player.h"

#define TARGET_PORT 5678

void runServer(Game *game);
void runClient(Game *game);

#endif