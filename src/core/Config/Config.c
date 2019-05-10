//
// Created by ananastia on 5/4/19.
//

#include "Config.h"


PlayerConfig *initPlayerConfig(){
    PlayerConfig *config=(PlayerConfig *)calloc(1, sizeof(PlayerConfig));
    config->mode=ONE_PLAYER;
    config->level = 1;
    return config;
}
