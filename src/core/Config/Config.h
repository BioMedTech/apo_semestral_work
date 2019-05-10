#ifndef MZAPO_TEMPLATE_CONFIG_H
#define MZAPO_TEMPLATE_CONFIG_H

#include "../Level/Level.h";
#include "../Player/Player.h";

enum PlayerMode {
    ONE_PLAYER,
    TWO_PLAYERS
} PlayerMode;

typedef struct PlayerConfig {
    enum PlayerMode mode;
    int level;
} PlayerConfig;

PlayerConfig *initPlayerConfig();

#endif //MZAPO_TEMPLATE_CONFIG_H
