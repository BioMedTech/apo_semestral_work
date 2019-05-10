//
// Created by ananastia on 5/4/19.
//

#include "Game.h"
#include "../lcd_logic/lcd_logic.h"

Game *initGame() {
    Game *newGame=(Game *) calloc(1, sizeof(Game));
    newGame->currentPlayer = initPlayer("Ananastia");
    newGame->boardConfig = initBoardConfig();
    return newGame;
}

// 1. Chose game mode
// 2. If 2 players=>seach for another player in net, of no player=>cancel
// 3. If two players, subscribe to the messages of the opponent 
// 4. Create random blok, start to move, reflect to knobs
// 5. When the bottom of the block is in the bottom, check if all row is filled, update score if needed create next