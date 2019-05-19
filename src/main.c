/*******************************************************************
  Simple program to check LCD functionality on MicroZed
  based MZ_APO board designed by Petr Porazil at PiKRON

  mzapo_lcdtest.c       - main and only file

  (C) Copyright 2004 - 2017 by Pavel Pisa
      e-mail:   pisa@cmp.felk.cvut.cz
      homepage: http://cmp.felk.cvut.cz/~pisa
      work:     http://www.pikron.com/
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "display/display.h"
#include "Game/Game.h"
#include "Server/server.h"

pthread_t game_thread, client_thread, server_thread, light_thread;

void *gameThread(void *vargp) {
    Game *game = (Game *) vargp;
    playGame(game);
}

void *lightThread(void *vargp) {
    Game *game = (Game *) vargp;
    while (1) {
        sleep(0.2);
    }
}

void initThreads(Game *game) {
    pthread_create(&game_thread, NULL, gameThread, game);
    pthread_create(&light_thread, NULL, lightThread, game);
}

void initServerThreads(Game *game) {
    pthread_create(&client_thread, NULL, runClient, game);
    pthread_create(&server_thread, NULL, runServer, game);
}

void joinThreads() {
    pthread_join(game_thread, NULL);
    pthread_join(light_thread, NULL);
}

void joinServerThreads() {
    pthread_join(server_thread, NULL);
    pthread_join(client_thread, NULL);
}

int main(int argc, char *argv[]) {
    Game *game = initGame();
    printMenu(game);

    if (game->mode == TWO_PLAYERS) {
        game->opponent = initPlayer();
        initServerThreads(game);
    }

    initThreads(game);
    joinServerThreads();
    joinThreads();
    
    freeData();

    return 0;
}
