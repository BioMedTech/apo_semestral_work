#ifndef MZAPO_TEMPLATE_CELL_H
#define MZAPO_TEMPLATE_CELL_H

#include <stdint.h>

#define GAME_FIELD_WIDTH 10
#define GAME_FIELD_HEIGHT 20

typedef struct Cell {
    uint16_t color;
    int state; //shows contents of the current cell: 0-nothing, 1-filled cell, 2-cell of currently falling figure
} Cell;

void fillCell(Cell *game_field, int row, int col, int color, int state);

void copyRow(Cell *game_field, int src_row, int dest_row);

Cell *getCell(Cell *game_field, int row, int col);

int getCellState(Cell *game_field, int row, int col);

#endif