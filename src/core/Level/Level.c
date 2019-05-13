//
// Created by ananastia on 5/4/19.
//

#include "Level.h"

Level **getAllLevels(){
    Level ** levels = (Level **)calloc(5, sizeof(Level *));
    for (int i=0; i < LEVEL_QUANTITY; i++){
        levels[i] = (Level *)calloc(1, sizeof(Level));
        levels[i]->number = i;
    }

    levels[0]->speed=INIT_SPEED;
    levels[1]->speed=INIT_SPEED * 2;
    levels[2]->speed=INIT_SPEED * 4;

    return levels;
}

void freeLevels(Level *** levels) {
    if (levels && * levels) {
        for (int i=0; i < LEVEL_QUANTITY; i++){
            free(*levels[i]);
        }   
        free(*levels);
        *levels = NULL;
    }
}
