#include "Player.h"

Player *initPlayer(){
    Player *player = (Player *) calloc(1, sizeof(Player));
    uuid_generate(player.id);
    player->username = "Ananastia";
    player->status=GAME_INIT;
    player->game_field = (Cell **) calloc(20, sizeof (Cell *));
    
    for (int i=0; i<20; i++){
        player->game_field[i] = (Cell *) calloc(15, sizeof(Cell));
    }
    return player;
}
