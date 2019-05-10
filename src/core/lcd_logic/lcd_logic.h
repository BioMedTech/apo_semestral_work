
#ifndef MZAPO_TEMPLATE_GRID_H
#define MZAPO_TEMPLATE_GRID_H

#define WIDTH 480
#define HEIGHT 320

#define HORIZONTAL_SIZE 30
#define VERTICAL_SIZE 20

#define CELL_SIZE 16

#include "../../mzapo_parlcd.h";
#include "../../mzapo_regs.h";
#include "../../font_types.h";
#include "Cell.h"



// every grid item should be 16*16
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
void redraw(unsigned char *parlcd_mem_base, Cell **playerField);
void fillCell(int row, int col, uint16_t color);
void drawLetter(char letter, int row, int column, uint16_t color, uint16_t bg);
void redrawData(unsigned char *parlcd_mem_base);

#endif //MZAPO_TEMPLATE_GRID_H
