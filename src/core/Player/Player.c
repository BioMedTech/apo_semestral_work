#include "Player.h"

Player *initPlayer(){
    Player *player = (Player *) calloc (1, sizeof(Player));
    player->status = GAME_INIT;
    player->game_field = (Cell **) calloc (GAME_FIELD_HEIGHT, sizeof (Cell *));
    
    memset(player->ip, '\0', sizeof(char) *16);

    for (int i = 0; i < GAME_FIELD_HEIGHT; i++)
    {
        player->game_field[i] = (Cell *) calloc (GAME_FIELD_WIDTH, sizeof(Cell));
    }
    return player;
}
