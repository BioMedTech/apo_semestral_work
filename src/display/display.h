
#ifndef MZAPO_TEMPLATE_GRID_H
#define MZAPO_TEMPLATE_GRID_H

#define WIDTH 480
#define HEIGHT 320

#define HORIZONTAL_SIZE 30
#define VERTICAL_SIZE 20

#define CELL_SIZE 16

#include <stdio.h>
#include <stdlib.h>
#include "../mzapo_parlcd.h"
#include "../mzapo_regs.h"
#include "../font_types.h"
#include "../Player/Player.h"



/*
      
*-------------*
| S |    G    |
| C |    A    |
| O |    M    |
| R |    E    |
| E |         |
|   |         |
*-------------*

*/

unsigned char *initDisplay();
void initData();
void redraw(unsigned char *parlcd_mem_base, Cell *playerField, Cell *opponentField);
void fillCell(int row, int col, uint16_t color);
void drawLetter(char letter, int row, int column, uint16_t color, uint16_t bg, int scale);
void redrawData(unsigned char *parlcd_mem_base);
void fillBgImg(char *filename);
void clearData();
void drawString(char *str, int row, int column, int16_t color, uint16_t bg, int scale);

#endif //MZAPO_TEMPLATE_GRID_H
