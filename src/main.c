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
    return NULL;
}

uint32_t convertToHex(uint16_t color)
{
    uint8_t r = (color >> 8) & 0xf8;
    uint8_t g= (color >> 3) & 0xfc;
    uint8_t b = (color << 3);
    return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}

void *lightThread(void *vargp)
{
    Game *game = (Game *)vargp;
    uint16_t curr_color = colors[1];
    // uint32_t curr_color = 0x0F000F0;
    
    while (!game->gameEnd)
    {
        curr_color+=0x0001;
        if (curr_color >= 0xFFFF){
            curr_color = colors[1];
        }
        // printf("Curr color: %X\n", curr_color);
        setLedValues(convertToHex(curr_color));
        sleep(1);
    }

    setLedValues(0xFF0000);
    sleep(1);
    setLedValues(0x0);
    return NULL;
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
    printf("/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\_/\\\\\\\\\\\\\\\\\\____/\\\\\\\\\\\\\\\\\\\\\\____/\\\\\\\\\\\\\\\\\\\\\\_______\n");
    printf("\\///////\\\\\\/////\\/\\\\\\///////////\\///////\\\\\\//////\\\\\\///////\\\\\\_\\/////\\\\\\///___/\\\\\\/////////\\\\\\____\n");
    printf("_______\\/\\\\\\_____\\/\\\\\\_________________\\/\\\\\\____\\/\\\\\\_____\\/\\\\\\_____\\/\\\\\\_____\\//\\\\\\______\\///____\n");
    printf("________\\/\\\\\\_____\\/\\\\\\\\\\\\\\\\\\\\\\_________\\/\\\\\\____\\/\\\\\\\\\\\\\\\\\\\\\\/______\\/\\\\\\______\\////\\\\\\__________\n");
    printf("_________\\/\\\\\\_____\\/\\\\\\///////__________\\/\\\\\\____\\/\\\\\\//////\\\\\\______\\/\\\\\\_________\\////\\\\\\______\n");
    printf("__________\\/\\\\\\_____\\/\\\\\\_________________\\/\\\\\\____\\/\\\\\\____\\//\\\\\\_____\\/\\\\\\____________\\////\\\\\\__\n");
    printf("___________\\/\\\\\\_____\\/\\\\\\_________________\\/\\\\\\____\\/\\\\\\_____\\//\\\\\\____\\/\\\\\\_____/\\\\\\______\\//\\\\\\\n");
    printf("____________\\/\\\\\\_____\\/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\_____\\/\\\\\\____\\/\\\\\\______\\//\\\\\\/\\\\\\\\\\\\\\\\\\\\\\\\///\\\\\\\\\\\\\\\\\\\\\\/\n");
    printf("_____________\\///______\\///////////////______\\///_____\\///________\\///\\///////////___\\///////////_\n");
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
    freeGame(game);

    return 0;
}
