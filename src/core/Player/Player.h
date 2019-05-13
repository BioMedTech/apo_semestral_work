#ifndef MZAPO_TEMPLATE_PLAYER_H
#define MZAPO_TEMPLATE_PLAYER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <uuid/uuid.h>

#include "../lcd_logic/Cell.h"

enum GameStatus {
    GAME_INIT,
    GAME_IN_PROGRESS,
    GAME_END
} GameStatus;


typedef struct Player {
    char *username;
    int score;
    struct Cell **game_field;
    struct uuid_t id;
    char ip[256];
    enum GameStatus status;
} Player;

Player *initPlayer();

#endif //MZAPO_TEMPLATE_PLAYER_H