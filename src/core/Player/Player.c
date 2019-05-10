#include "Player.h"

Player *initPlayer(char *username){
    Player *player = (Player *) calloc(1, sizeof(Player));
    player->username = username;
    return player;
}


