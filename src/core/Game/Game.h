#ifndef MZAPO_TEMPLATE_GAME_H
#define MZAPO_TEMPLATE_GAME_H

#include <stdio.h>
#include "../Config/Config.h"
#include "../Player/Player.h"


typedef struct Game {
    struct PlayerConfig * playerConfig;
    struct BoardConfig *boardConfig;
    struct Player * currentPlayer;
    struct Player * opponent;
} Game;

Game *initGame();

#endif //MZAPO_TEMPLATE_GAME_H
