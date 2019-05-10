#ifndef MZAPO_TEMPLATE_CELL_H
#define MZAPO_TEMPLATE_CELL_H
#include <stdint.h>

typedef struct Cell
{
    uint16_t color;
    int state; //0-nothing, 1-filled cell, 2-cell of current figure
} Cell;

#endif