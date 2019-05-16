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
#include "core/lcd_logic/lcd_logic.h"
#include "core/Game/Game.h"
#include "core/knobs_logics/knobs_logic.h"
#include "core/Figure/Figure.h"
#include "core/Server/server.h"

pthread_t game_thread, client_thread, server_thread, light_thread;
pthread_mutex_t display_mutex, light_mutex;

unsigned char *mem;

void *gameThread(void *vargp) {
    Game *game = (Game *)vargp;
    playGame(game);
}

void *lightThread(){
    while(1){
        // Check if the value of light was changed
        sleep(3);
    }
}

void initTreads(Game *game) {
    pthread_create(&game_thread, NULL, gameThread, game);
    pthread_create(&server_thread, NULL, runServer, game);
}

void initServerThreads(Game *game){
    pthread_create(&client_thread, NULL, runClient, game);
    pthread_create(&light_thread, NULL, lightThread, NULL);
}

void initMutex(){
    pthread_mutex_init(&display_mutex, NULL);
    pthread_mutex_init(&light_mutex, NULL);
}

void joinThreads(){
    pthread_join(game_thread, NULL);
    pthread_join(light_thread, NULL);
}

void joinServerThreads(){
    pthread_join(server_thread, NULL);
    pthread_join(client_thread, NULL);
}

int main(int argc, char *argv[]) {
    Game *game = initGame();
    printMenu(game);
    printf("\n------\n------\n------\n   ||\n   ||\n   ||\n    -\nGame was initalized\n");

    initMutex();
 
    if (game->currentPlayer->mode == TWO_PLAYERS){
        initServerThreads(game);
        joinServerThreads();
    }

    initTreads(game);
    joinThreads();
   
    return 0;
}
