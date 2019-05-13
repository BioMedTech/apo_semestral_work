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


uint32_t *first_led;
uint32_t *second_led;
uint32_t *led_controller;

pthread_t game_thread;
pthread_t client_thread;
pthread_t server_thread;

unsigned char *mem;

void lights_init()
{
    led_controller = map_phys_address(SPILED_REG_BASE_PHYS + SPILED_REG_LED_KBDWR_DIRECT_o, 4, 0);
    // first_led = map_phys_address(SPILED_REG_BASE_PHYS +  SPILED_REG_LED_KBDWR_DIRECT_o, 2, 0);
    // second_led = map_phys_address(SPILED_REG_BASE_PHYS +  SPILED_REG_LED_KBDWR_DIRECT_o + 2, 2, 0);

}

void *gameThread(void *vargp) {
    Game *game = (Game *)vargp;
    playGame(game);
}


void initTreads(Game *game) {
    pthread_create(&game_thread, NULL, gameThread, game);
    pthread_create(&server_thread, NULL, runServer, game);
    pthread_create(&client_thread, NULL, runClient, game);
}

void endThreads(){
    pthread_join(game_thread, NULL);
    pthread_join(server_thread, NULL);
    pthread_join(client_thread, NULL);
}

void set_lights(uint32_t val) {
    *led_controller = val;
    *(led_controller + 2) = val;
    printf("%x\n", *led_controller);
    printf("%x\n", *led_controller + 2);
}


int main(int argc, char *argv[]) {
    Game *game = initGame();
    printMenu(game);
    printf("\nGame was initalized\n");

    if (game->currentPlayer->mode==TWO_PLAYERS){
        initTreads(game);
        endThreads();
    } else {
        playGame(game);
    }
    
    return 0;
}
