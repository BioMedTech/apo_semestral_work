#ifndef MZAPO_TEMPLATE_GAME_H
#define MZAPO_TEMPLATE_GAME_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "../../mzapo_regs.h"
#include "../../mzapo_parlcd.h"
#include "../../mzapo_phys.h"

#include "../lcd_logic/lcd_logic.h"
#include "../Player/Player.h"
#include "../Figure/Figure.h"
#include "../knobs_logics/knobs_logic.h"

#define RED 0xF800
#define GREEEN 0x07E0
#define BLUE 0x1535
#define ORANGE 0xF520

enum GameMode
{
    ONE_PLAYER,
    TWO_PLAYERS
} GameMode;

typedef struct Game {
    struct Player *currentPlayer;
    struct Player *opponent;
    struct Figure *currentFigure;
    struct Figure *nextFigure;
    enum GameMode mode;
} Game;

typedef struct PersonRating{
    char time[12];
    int score;
    char ip[16];
}PersonRating;

Game *initGame();
void playGame(Game *game);
void printMenu(Game *game);

#endif //MZAPO_TEMPLATE_GAME_H
