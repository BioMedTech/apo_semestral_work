#include "Player.h"

Player *initPlayer(){
    Player *player = (Player *) calloc (1, sizeof(Player));
    player->status = GAME_INIT;
    player->game_field = (Cell *) calloc (GAME_FIELD_HEIGHT * GAME_FIELD_WIDTH, sizeof (Cell));

    memset(player->ip, '\0', sizeof(char) * 16);
    return player;
}

Cell *getCell(Cell *game_field, int row, int col) {
    return &game_field[row * GAME_FIELD_WIDTH + col];
}

void fillGameField(Cell *game_field, int row, int col, int color, int state)
{
    Cell *cell = getCell(game_field, row, col);
    cell->color = color;
    cell->state = state;
}

int getCellState(Cell *game_field, int row, int col)
{
    return getCell(game_field, row, col)->state;
}

void copyRow(Cell *game_field, int src_row, int dest_row){
    memcpy(&game_field[dest_row*GAME_FIELD_WIDTH], &game_field[src_row*GAME_FIELD_WIDTH], sizeof(Cell)*GAME_FIELD_WIDTH);
}