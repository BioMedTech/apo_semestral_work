#ifndef MZAPO_TEMPLATE_PLAYER_H
#define MZAPO_TEMPLATE_PLAYER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct Player {
    char *username;
    int score;
} Player;

Player *initPlayer(char *username);

#endif //MZAPO_TEMPLATE_PLAYER_H