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


uint32_t *first_led;
uint32_t *second_led;
uint32_t *led_controller;

pthread_t game_thread;
pthread_t knobs_thread;
pthread_t server_thread;

Figure *currentFigure;
unsigned char *mem;

void lights_init()
{
    led_controller = map_phys_address(SPILED_REG_BASE_PHYS + SPILED_REG_LED_KBDWR_DIRECT_o, 4, 0);
    // first_led = map_phys_address(SPILED_REG_BASE_PHYS +  SPILED_REG_LED_KBDWR_DIRECT_o, 2, 0);
    // second_led = map_phys_address(SPILED_REG_BASE_PHYS +  SPILED_REG_LED_KBDWR_DIRECT_o + 2, 2, 0);

}

void *gameThread(void *vargp) {
    Game *game = (Game *)vargp;
    printMenu();
    // playGame(game);
}

void *serverThread(void *vargp) {
    sleep(1);
    while(1){
        // printf("Printing from Thread 3\n");
        sleep(2);
    }
    return NULL;
}


void initTreads(Game *game) {
    pthread_create(&game_thread, NULL, gameThread, game);
    // pthread_create(&knobs_thread, NULL, knobsThread, game);
    pthread_create(&server_thread, NULL, serverThread, game);
}

void endThreads(){
    pthread_join(game_thread, NULL);
    // pthread_join(knobs_thread, NULL);
    pthread_join(server_thread, NULL);

}

void set_lights(uint32_t val) {
    *led_controller = val;
    *(led_controller + 2) = val;
    printf("%x\n", *led_controller);
    printf("%x\n", *led_controller + 2);
}


int main(int argc, char *argv[]) {
    // initData();
    Game *game = initGame();
    printf("\nGame was initalized\n");
    
    initTreads(game);
    endThreads();
        // while(1){
        //     getKnobsValue(knob_mem);
        // }

        // data=calloc(3, sizeof(int));
        // Game *game = initGame();

        // initTreads(game);
        // endThreads(game);

        // pthread_attr_t attr;
        // pthread_attr_init(&attr);
        // pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        //    lights_init();
        //    set_lights(0x252525);
        //    uint32_t *knobs = map_phys_address(SPILED_REG_BASE_PHYS + SPILED_REG_KNOBS_8BIT_o, 4, 0);

        // while(1){
        //  printf("%x\n", *knobs);
        // //  set_lights(*knobs);
        // }

        // unsigned char *parlcd_mem_base;
        // int i, j;
        // unsigned c;

        // parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

        // if (parlcd_mem_base == NULL)
        //   exit(1);

        // parlcd_hx8357_init(parlcd_mem_base);

        // parlcd_write_cmd(parlcd_mem_base, 0x2c);
        // for (i = 0; i < 320 ; i++) {
        //   for (j = 0; j < 480 ; j++) {
        //     c = 0;
        //     parlcd_write_data(parlcd_mem_base, c);
        //   }
        // }

        // parlcd_write_cmd(parlcd_mem_base, 0x2c);
        // for (i = 0; i < 320 ; i++) {
        //   for (j = 0; j < 480 ; j++) {
        //     c = ((i & 0x1f) << 11) | (j & 0x1f);
        //     if (i < 10)
        //       c |= 0x3f << 5;
        //     if (j < 10)
        //       c |= 0x3f << 5;
        //     parlcd_write_data(parlcd_mem_base, c);
        //   }
        // }

        // while (0) {
        //    struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 200 * 1000 * 1000};

        //    *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = 0x0001;
        //    *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = 0x0002;
        //    *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = 0x0004;
        //    *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = 0x0008;
        //    *(volatile uint32_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = 0x0010;
        //    *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = 0x0020;

        //    clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
        // }

        return 0;
}
