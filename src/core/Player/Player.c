#include "Player.h"

Player *initPlayer(){
    Player *player = (Player *) calloc(1, sizeof(Player));
    player->username = "Ananastia";
    player->game_field = (Cell **) calloc(20, sizeof (Cell *));
    
    for (int i=0; i<20; i++){
        player->game_field[i] = (Cell *) calloc(15, sizeof(Cell));
    }
    return player;
}
