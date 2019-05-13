//
// Created by ananastia on 5/4/19.
//

#include "Game.h"
unsigned char *mem;
unsigned char *mem_base;

Game *initGame()
{
    mem = initDisplay();
    Game *newGame = (Game *)calloc(1, sizeof(Game));
    newGame->currentPlayer = initPlayer();
    newGame->currentFigure = initRandomFigure(newGame->currentPlayer->game_field);
    newGame->playerConfig = initPlayerConfig();
    printMenu(newGame);

    return newGame;
}


void printMenu(Game *game){
    initData();
    mem_base = init_knobs();

    uint16_t color_one_player = BLUE;
    uint16_t color_two_players = 0x0;
    int one_player_chosen = 1;
    int is_pressing = 0;

    Knobs_state state = getKnobsValue(mem_base);
    
    while(1){
        Knobs_state state = getKnobsValue(mem_base);

        drawString("TETRIS", 3, 8, RED, 0x0);
        
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
        
        if (state.rb == 1) {
           game->playerConfig->mode = one_player_chosen?ONE_PLAYER : TWO_PLAYERS;
           break;   
        }
        
        drawString("ONE PLAYER", 5, 6, RED, color_one_player);
        drawString("TWO PLAYERS", 7, 8, RED, color_two_players);

        redrawData(mem);
        sleep(0.006);
    }
    
    uint16_t colors[3];
    int level=0;
    is_pressing = 0;

    while (1)
    {
        Knobs_state state = getKnobsValue(mem_base);

        drawString("TETRIS", 3, 8, RED, 0x0);
        drawString("LEVEL", 4, 8, RED, 0x0);

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

        if (state.rb == 1)
        {
            game->playerConfig->level = level % 3;
            break;
        }
        drawString("EASY", 5, 8, 0xFFFF, colors[0]);
        drawString("MEDIUM", 6, 6, 0xFFFF, colors[1]);
        drawString("HARD", 7, 8, 0xFFFF, colors[2]);

        redrawData(mem);
        parlcd_delay(16);
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
    Knobs_state state = getKnobsValue(mem_base);
    Knobs_state new_state = getKnobsValue(mem_base);
    int success = 1;

    game->currentPlayer->status = GAME_IN_PROGRESS;

    while (1)
    {
        if (new_state.bk - state.bk >= 2)
        {
            moveFigure(1, 0, game->currentFigure, game->currentPlayer->game_field);
            state.bk = new_state.bk;  
        } else if (state.bk - new_state.bk >= 2)
        {
            moveFigure(-1, 0, game->currentFigure, game->currentPlayer->game_field);
            state.bk = new_state.bk;  
        }

        if (new_state.rk - state.rk >= 2 || state.rk - new_state.rk >= 2) {
            rotateFigure(game->currentFigure, new_state.rk > state.rk, game->currentPlayer->game_field);
            state.rk = new_state.rk;  
        }

        if (new_state.gb)
            success = moveFigure(0, 1, game->currentFigure, game->currentPlayer->game_field);
        

        clock_t diff = clock() - before;
        int mces = diff * 1000 / CLOCKS_PER_SEC;
       
        if (mces > 1000 && success)
        {
            success = moveFigure(0, 1, game->currentFigure, game->currentPlayer->game_field);
            before = clock();
        } 

        new_state = getKnobsValue(mem_base);
        redraw(mem, game->currentPlayer->game_field);
      
        if (!success)
        {
            Figure *new_figure = initRandomFigure(game->currentPlayer->game_field);
            memcpy(game->currentFigure, new_figure, sizeof(Figure));
            free(new_figure);
            // printf("Figure changed\n");
        }
        
        success = 1;

        // struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 160 * 1000};
        // clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
        sleep(0.006);
    }

    game->status = GAME_END;

    return NULL;
}
