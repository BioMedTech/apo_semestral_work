//
// Created by ananastia on 5/4/19.
//

#ifndef MZAPO_TEMPLATE_LEVEL_H
#define MZAPO_TEMPLATE_LEVEL_H

#define LEVEL_QUANTITY 5
#define INIT_SPEED 20

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct Level{
    int number;
    int speed;
} Level;

Level **getAllLevels();
void freeLevels(Level *** levels);

#endif //MZAPO_TEMPLATE_LEVEL_H
