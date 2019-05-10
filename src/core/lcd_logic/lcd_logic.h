
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
void redraw(unsigned char *parlcd_mem_base);
void fillCell(int row, int col, uint16_t color);


#endif //MZAPO_TEMPLATE_GRID_H
