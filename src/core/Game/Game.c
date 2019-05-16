//
// Created by ananastia on 5/4/19.
//

#include "Game.h"
unsigned char *mem;

Game *initGame()
{
    mem = initDisplay();
    Game *newGame = (Game *)calloc(1, sizeof(Game));
    newGame->currentPlayer = initPlayer();
    newGame->currentFigure = initRandomFigure(newGame->currentPlayer->game_field);
    newGame->nextFigure = initRandomFigure(newGame->currentPlayer->game_field);

    return newGame;
}


void printMenu(Game *game){
    initData();
    init_knobs();

    uint16_t color_one_player = BLUE;
    uint16_t color_two_players = 0x0;
    int one_player_chosen = 1;
    int is_pressing = 0;

    Knobs_state state = getKnobsValue();
    int _continue = 1;

    clearData();
    fillBgImg("/tmp/mozguana/logo.ppm");
    drawString("TETRIS", 3, 8, RED, 0x0);
    redrawData(mem);

    while(_continue){
        Knobs_state state = getKnobsValue();
        
        if (state.gb == 1 && !is_pressing) {
            if (one_player_chosen){
                color_one_player = 0X0;
                color_two_players = BLUE;
            } else {
                color_one_player = BLUE;
                color_two_players = 0X0;
            }
            one_player_chosen = !one_player_chosen;
            is_pressing = 1;    
        } else if (!state.gb){
            is_pressing = 0;
        }
        
        drawString("ONE PLAYER", 5, 6, RED, color_one_player);
        drawString("TWO PLAYERS", 7, 6, RED, color_two_players);


        if (state.rb == 1) {
           game->currentPlayer->mode = one_player_chosen ? ONE_PLAYER : TWO_PLAYERS;
           _continue = 0;
        }

        redrawData(mem);
        parlcd_delay(0.0002);
    }
    
    clearData();
    redrawData(mem);

    uint16_t colors[3];
    colors[0] = BLUE;
    colors[1] = 0x0;
    colors[2] = 0x0;
   
    int level=0;
    is_pressing = 0;
    _continue = 1;
    int unlock = 0;

    drawString("TETRIS", 3, 8, RED, 0x0);
    drawString("LEVEL", 4, 8, RED, 0x0);

    while (_continue)
    {
        Knobs_state state = getKnobsValue();

        if (state.gb == 1 && !is_pressing)
        {
            level=(level+1) % 3;
            for (int i=0; i<3; i++){
                if (i==level){
                    colors[i] = BLUE;
                } else {
                    colors[i] = 0x0;
                }
            }
            is_pressing = 1;
        }
        else if (!state.gb)
        {
            is_pressing = 0;
        }

       
        drawString("EASY", 5, 8, 0xFFFF, colors[0]);
        drawString("MEDIUM", 6, 6, 0xFFFF, colors[1]);
        drawString("HARD", 7, 8, 0xFFFF, colors[2]);

        redrawData(mem);
        parlcd_delay(0.0002);

        if (!unlock && !state.rb) unlock = 1;

        if (state.rb && unlock)
        {
            game->currentPlayer->level = level % 3;
            _continue = 0;
        }
    }
}



// 1. Chose game mode
// 2. If 2 players=>seach for another player in net, of no player=>cancel
// 3. If two players, subscribe to the messages of the opponent
// 4. Create random blok, start to move, reflect to knobs
// 5. When the bottom of the block is in the bottom, check if all row is filled, update score if needed create next


void playGame(Game *game)
{
    sleep(1);
    clock_t before = clock();
    Knobs_state state = getKnobsValue();
    Knobs_state new_state = getKnobsValue();
    int success = 1;
    int period = 800;

    switch (game->currentPlayer->level){
        case 0:
          break;
        case 1:
          period = 650;
          break;
        default:
          period = 500;
          
    }

    game->currentPlayer->status = GAME_IN_PROGRESS;
    int _continue=1;

    while (_continue)
    {
        if (new_state.gk - state.gk >= 3)
        {
            moveFigure(1, 0, game->currentFigure, game->currentPlayer);
            state.gk = new_state.gk;
        } else if (state.gk - new_state.gk >= 3)
        {
            moveFigure(-1, 0, game->currentFigure, game->currentPlayer);
            state.gk = new_state.gk;  
        }

        if (new_state.rk - state.rk >= 3 || state.rk - new_state.rk >= 3) {
            rotateFigure(game->currentFigure, new_state.rk > state.rk, game->currentPlayer->game_field);
            state.rk = new_state.rk;  
        }

        if (new_state.gb)
            success = moveFigure(0, 1, game->currentFigure, game->currentPlayer);
        

        clock_t diff = clock() - before;
        int mces = diff * 1000 / CLOCKS_PER_SEC;
       
        if (mces > period && success)
        {
            success = moveFigure(0, 1, game->currentFigure, game->currentPlayer);
            before = clock();
        } 

        new_state = getKnobsValue();
        // pthread_mutex_lock(display_mutex);
        redraw(mem, game->currentPlayer->game_field);
      
        if (!success)
        {
            if (willCollide(game->currentFigure, 0, 1, game->currentPlayer->game_field)){
                _continue=0;
            } else {
                Figure *new_figure = initRandomFigure(game->currentPlayer->game_field);
                memcpy(game->currentFigure, game->nextFigure, sizeof(Figure));
                memcpy(game->nextFigure, new_figure, sizeof(Figure));
                free(new_figure);
            }
            success = 1;
        }
        
        parlcd_delay(20);
    }

    game->currentPlayer->status = GAME_END;

    char buffer[200];
    snprintf(buffer, sizeof buffer, "YOUR SCORE IS %d\0", game->currentPlayer->score);
    printf("SCORE: %d\n", game->currentPlayer->score);

    clearData();
    drawString("END OF THE GAME!", 7, 8, 0xFFFF, 0x0);
    drawString(buffer, 10, 8, 0xFFFF, 0x0);
    setLedValues(0xFF0000);
    redrawData(mem);

    return NULL;
}
