#ifndef MZAPO_TEMPLATE_PTHREAD_ARGS_H
#define MZAPO_TEMPLATE_PTHREAD_ARGS_H

#include "./Game/Game.h"

typedef struct pthread_args {
    struct Game *game;
    pthread_mutex_t game_mutex;
    pthread_mutex_t display_mutex;
    pthread_mutex_t light_mutex;
} pthread_args;

#endif