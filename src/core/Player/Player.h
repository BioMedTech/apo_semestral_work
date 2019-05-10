#ifndef MZAPO_TEMPLATE_PLAYER_H
#define MZAPO_TEMPLATE_PLAYER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../lcd_logic/Cell.h"


typedef struct Player {
    char *username;
    int score;
    struct Cell **game_field;
} Player;

Player *initPlayer();

#endif //MZAPO_TEMPLATE_PLAYER_H