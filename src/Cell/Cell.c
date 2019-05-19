#include "Cell.h"

/*
 * Get cell from array of cells
 */
Cell *getCell(Cell *game_field, int row, int col) {
    return &game_field[row * GAME_FIELD_WIDTH + col];
}
/*
 * Fill cell with state nd color
 */
void fillCell(Cell *game_field, int row, int col, int color, int state) {
    Cell *cell = getCell(game_field, row, col);
    cell->color = color;
    cell->state = state;
}
/*
 * Get state of the cell
 */

int getCellState(Cell *game_field, int row, int col) {
    return getCell(game_field, row, col)->state;
}
/*
 * Replace ${dest_row} of the game_field with ${src_row}
 */
void copyRow(Cell *game_field, int src_row, int dest_row) {
    memcpy(&game_field[dest_row * GAME_FIELD_WIDTH], &game_field[src_row * GAME_FIELD_WIDTH],
           sizeof(Cell) * GAME_FIELD_WIDTH);
}