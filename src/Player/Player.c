#include "Player.h"

/*
 * Init player with empty game field
 */

Player *initPlayer(){
    Player *player = (Player *) calloc (1, sizeof(Player));
    player->status = GAME_INIT;
    player->game_field = (Cell *) calloc (GAME_FIELD_HEIGHT * GAME_FIELD_WIDTH, sizeof (Cell));
    player->score = 0;

    memset(player->ip, '\0', sizeof(char) * 16);
    return player;
}

void freePlayer(Player *player){
    free(player->game_field);
    free(player);
}