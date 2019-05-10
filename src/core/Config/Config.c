//
// Created by ananastia on 5/4/19.
//

#include "Config.h"


PlayerConfig *initPlayerConfig(){
    PlayerConfig *config=(PlayerConfig *)calloc(1, sizeof(PlayerConfig));
    return config;
}

BoardConfig *initBoardConfig(){
    BoardConfig *config = (BoardConfig *) calloc(1, sizeof(BoardConfig));
    uint16_t **data = initData();
    unsigned char *parlcd_mem_base = initDisplay();
    config->data = data;
    config->parlcd_mem_base = parlcd_mem_base;
    return config;
}