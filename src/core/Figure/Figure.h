//
// Created by ananastia on 5/4/19.
//

#ifndef MZAPO_TEMPLATE_FIGURE_H
#define MZAPO_TEMPLATE_FIGURE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../lcd_logic/lcd_logic.h"

#define FIGURE_TYPES_QUANTITY 7
#define FIGURE_CELL_QUANTITY 4

typedef struct Coords{
   int x;
   int y;
} Coords;


typedef struct Figure {
   int type;           
   uint32_t color;
   struct Coords offset;
   struct Coords state[4];
} Figure;

void initRandomFigure();
// void initRandomFigure()
/*

DEFAULT FIGURES:

3       2      0     1      4     5     6 

**      *      **    *     **     **    ** 
*      ***     **    *      *    **      **
*                    *      *
                     *

( 0 -1) ( 1 -1)  **    figure->offset - real coords of point (0, 0)
( 0  0)          *     figure->state - coords of figure in 3*3 matrix (0, 0) - (1, 1) 
( 0  1)          *     figure->type - type of figure
*/

#endif //MZAPO_TEMPLATE_FIGURE_H
