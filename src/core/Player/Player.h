#ifndef MZAPO_TEMPLATE_PLAYER_H
#define MZAPO_TEMPLATE_PLAYER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "../lcd_logic/Cell.h"

enum GameStatus {
    GAME_INIT,
    GAME_IN_PROGRESS,
    GAME_END
} GameStatus;


typedef struct Player {
    int score;
    struct Cell *game_field;
    char ip[16];
    enum GameStatus status;
    int level;
} Player;

Player *initPlayer();
void fillGameField(Cell *game_field, int row, int col, int color, int state);
void copyRow(Cell *game_field, int src_row, int dest_row);
Cell *getCell(Cell *game_field, int row, int col);
 int getCellState(Cell *game_field, int row, int col);

#endif //MZAPO_TEMPLATE_PLAYER_H