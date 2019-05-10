//
// Created by ananastia on 5/4/19.
//

#include "Game.h"
unsigned char *mem;
unsigned char *mem_base;

    Game * initGame()
{
    mem = initDisplay();
    printMenu();
    Game *newGame = (Game *)calloc(1, sizeof(Game));
    newGame->currentPlayer = initPlayer();
    newGame->currentFigure = initRandomFigure(newGame->currentPlayer->game_field);
    newGame->playerConfig = initPlayerConfig();

    return newGame;
}


void printMenu(){
    initData();
    mem_base = init_knobs();

    uint16_t color_one_player = BLUE;
    uint16_t color_two_players = 0x0;
    int one_player_chosen = 1;
    int is_pressing = 0;

    Knobs_state state = getKnobsValue(mem_base);
    
    while(1){
        Knobs_state state = getKnobsValue(mem_base);

        drawLetter('T', 3, 8, RED, 0x0);
        drawLetter('E', 3, 9, RED, 0x0);
        drawLetter('T', 3, 10, RED, 0x0);
        drawLetter('R', 3, 11, RED, 0x0);
        drawLetter('I', 3, 12, RED, 0x0);
        drawLetter('S', 3, 13, RED, 0x0);
        
        if (state.gb == 1 && !is_pressing) {
            if (one_player_chosen){
                color_one_player = 0X0;
                color_two_players = BLUE;
            } else {
                color_one_player = BLUE;
                color_two_players = 0X0;
            }
            one_player_chosen=!one_player_chosen;
            is_pressing = 1;    
        } else if (!state.gb){
            is_pressing = 0;
        }
        
        if (state.rb == 1) {
           break;   
        }
        
        drawLetter('O', 5, 6, RED, color_one_player);
        drawLetter('N', 5, 7, RED, color_one_player);
        drawLetter('E', 5, 8, RED, color_one_player);
        drawLetter(' ', 5, 9, RED, color_one_player);
        drawLetter('P', 5, 10, RED, color_one_player);
        drawLetter('L', 5, 11, RED, color_one_player);
        drawLetter('A', 5, 12, RED, color_one_player);
        drawLetter('Y', 5, 13, RED, color_one_player);
        drawLetter('E', 5, 14, RED, color_one_player);
        drawLetter('R', 5, 15, RED, color_one_player);

        drawLetter('T', 7, 6, RED, color_two_players);
        drawLetter('W', 7, 7, RED, color_two_players);
        drawLetter('O', 7, 8, RED, color_two_players);
        drawLetter(' ', 7, 9, RED, color_two_players);
        drawLetter('P', 7, 10, RED, color_two_players);
        drawLetter('L', 7, 11, RED, color_two_players);
        drawLetter('A', 7, 12, RED, color_two_players);
        drawLetter('Y', 7, 13, RED, color_two_players);
        drawLetter('E', 7, 14, RED, color_two_players);
        drawLetter('R', 7, 15, RED, color_two_players);
        drawLetter('S', 7, 16, RED, color_two_players);
        redrawData(mem);
        sleep(0.006);
    }
    
    uint16_t colors[3];
    int level=0;
    is_pressing = 0;

    while (1)
    {
        Knobs_state state = getKnobsValue(mem_base);

        drawLetter('T', 3, 8, RED, 0x0);
        drawLetter('E', 3, 9, RED, 0x0);
        drawLetter('T', 3, 10, RED, 0x0);
        drawLetter('R', 3, 11, RED, 0x0);
        drawLetter('I', 3, 12, RED, 0x0);
        drawLetter('S', 3, 13, RED, 0x0);

        drawLetter('L', 4, 8, RED, 0x0);
        drawLetter('E', 4, 9, RED, 0x0);
        drawLetter('V', 4, 10, RED, 0x0);
        drawLetter('E', 4, 11, RED, 0x0);
        drawLetter('L', 4, 12, RED, 0x0);

        if (state.gb == 1 && !is_pressing)
        {
            level=(level+1) % 3;
            for (int i=0; i<3; i++){
                if (i==level){
                    colors[i]=BLUE;
                }else{
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
            break;
        }

        drawLetter('E', 5, 7, 0xFFFF, colors[0]);
        drawLetter('A', 5, 8, 0xFFFF, colors[0]);
        drawLetter('S', 5, 9, 0xFFFF, colors[0]);
        drawLetter('Y', 5, 10, 0xFFFF, colors[0]);
        
        drawLetter('M', 6, 7, 0xFFFF, colors[1]);
        drawLetter('E', 6, 8, 0xFFFF, colors[1]);
        drawLetter('D', 6, 9, 0xFFFF, colors[1]);
        drawLetter('I', 6, 10, 0xFFFF, colors[1]);
        drawLetter('U', 6, 11, 0xFFFF, colors[1]);
        drawLetter('M', 6, 12, 0xFFFF, colors[1]);

        drawLetter('H', 7, 7, 0xFFFF, colors[2]);
        drawLetter('A', 7, 8, 0xFFFF, colors[2]);
        drawLetter('R', 7, 9, 0xFFFF, colors[2]);
        drawLetter('D', 7, 10, 0xFFFF, colors[2]);

        redrawData(mem);
        sleep(0.006);
    }
}


void playGame(Game *game)
{
    sleep(1);
    int change_move_state = 0;
    int change_rotation_state = 0;

    clock_t before = clock();
    Knobs_state state = getKnobsValue(mem_base);
    Knobs_state new_state = getKnobsValue(mem_base);
    int success = 1;

    while (1)
    {
        if (new_state.bk - state.bk >= 2)
        {
            moveFigure(1, 0, game->currentFigure, game->currentPlayer->game_field);
            change_move_state = 1;
        }
        else if (state.bk - new_state.bk >= 2)
        {
            moveFigure(-1, 0, game->currentFigure, game->currentPlayer->game_field);
            change_move_state = 1;
        }

        if (new_state.rk - state.rk >= 2)
        {
            rotateFigure(game->currentFigure, 1, game->currentPlayer->game_field);
            change_rotation_state = 1;
        }
        else if (state.rk - new_state.rk >= 2)
        {
            rotateFigure(game->currentFigure, 0, game->currentPlayer->game_field);
            change_rotation_state = 1;
        }

        if (new_state.gb)
        {
            success = moveFigure(0, 1, game->currentFigure, game->currentPlayer->game_field);
        }

        clock_t diff = clock() - before;

        int mces = diff * 1000 / CLOCKS_PER_SEC;
       
        if (mces > 1000 && success)
        {
            success = moveFigure(0, 1, game->currentFigure, game->currentPlayer->game_field);
            before = clock();
        }

        if (change_move_state)
            state.bk = new_state.bk;

        if (change_rotation_state)
            state.rk = new_state.rk;


        new_state = getKnobsValue(mem_base);
        redraw(mem, game->currentPlayer->game_field);
        // sleep(0.016);
        if (!success)
        {
            Figure *new_figure = initRandomFigure(game->currentPlayer->game_field);
            memcpy(game->currentFigure, new_figure, sizeof(Figure));
            free(new_figure);
            printf("Figure changed\n");
        }

        change_move_state = 0;
        change_rotation_state = 0;
        success = 1;

        // struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 160 * 1000};
        // clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
        sleep(0.006);
    }
    return NULL;
}


// 1. Chose game mode
// 2. If 2 players=>seach for another player in net, of no player=>cancel
// 3. If two players, subscribe to the messages of the opponent
// 4. Create random blok, start to move, reflect to knobs
// 5. When the bottom of the block is in the bottom, check if all row is filled, update score if needed create next
