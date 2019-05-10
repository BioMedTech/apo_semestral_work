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
    struct Level level;
} PlayerConfig;



typedef struct BoardConfig {
    uint16_t **data;
    unsigned char *parlcd_mem_base;
} BoardConfig;

#endif //MZAPO_TEMPLATE_CONFIG_H
