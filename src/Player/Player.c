#include "Player.h"

/*
 * Init player with empty game field
 */

Player *initPlayer(){
    Player *player = (Player *) calloc (1, sizeof(Player));
    player->status = GAME_INIT;
    player->game_field = (Cell *) calloc (GAME_FIELD_HEIGHT * GAME_FIELD_WIDTH, sizeof (Cell));

    memset(player->ip, '\0', sizeof(char) * 16);
    return player;
}

