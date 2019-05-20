//
// Created by ananastia on 5/4/19.
//

#include "Game.h"

unsigned char *mem;

// Init display and game with one player
Game *initGame() {
    mem = initDisplay();
    Game *newGame = (Game *) calloc(1, sizeof(Game));
    newGame->currentPlayer = initPlayer();

    return newGame;
}

void printPersonRating(FILE *file, PersonRating rating, int place, char buffer[200], uint16_t col) {
    snprintf(buffer, 200 * sizeof(char), "%d. DATE: %s, IP: %s, SCORE: %d", place, rating.time, rating.ip,
             rating.score);
    drawString(buffer, 3 + place, 4, col, 0x0, 1);
    fwrite(&rating, sizeof(PersonRating), 1, file);
}


void showRatingTable(Player *currentPlayer) {
    int lines = 0;
    FILE *file = fopen("rating.txt", "w+");
    fscanf(file, "%d", &lines);
    PersonRating *rating = (PersonRating *) calloc(lines, sizeof(PersonRating));

    int myPlace = lines + 1;

    if (lines > 0) {
        for (int i = 0; i < lines; i++) {

            fread(&rating[i], sizeof(PersonRating), 1, file);

            if (rating[i].score > currentPlayer->score) {
                myPlace = i + 1;
            }
        }
    }

    PersonRating me;

    time_t now;
    struct tm *tme = localtime(time(&now));

    char buffer[200];
    snprintf(me.time, 12 * sizeof(char), "%.2d.%.2d.%d\0", tme->tm_mday, tme->tm_mon + 1, tme->tm_year + 1900);
    me.score = currentPlayer->score;
    strncpy(me.ip, currentPlayer->ip, 15);

    fprintf(file, "%d", lines);

    for (int i = 0; i < lines + 1; i++) {
        if (i == myPlace) {
            printPersonRating(file, me, i, buffer, RED);
        } else {
            printPersonRating(file, rating[i], i, buffer, 0x0);
        }
    }

    free(rating);
    fclose(file);
}

/*
 * Prints menu:
 * 1. Number of players(1 or 2)
 * 2. Level of difficulty
 */
void printMenu(Game *game) {
    initData();
    init_knobs();

    int one_player_chosen = 1;
    int is_pressing = 0;

    Knobs_state state;
    int _continue = 1;

    // Print label picture
    clearData();
    fillBgImg("/tmp/mozguana/logo.ppm");
    redrawData(mem);

    while (_continue) {
        state = getKnobsValue();

        // If green button is pressing and was not in pressed state yet, refres selection
        if (state.gb == 1 && !is_pressing) {
            one_player_chosen = !one_player_chosen;
            is_pressing = 1;
        } else if (!state.gb) {
            is_pressing = 0;
        }

        // Fill blue background of chosen mode
        drawString("ONE PLAYER", 7, 2, 0xFFFF, one_player_chosen ? BLUE : 0x0, 2);
        drawString("TWO PLAYERS", 7, 15, 0xFFFF, one_player_chosen ? 0x0 : BLUE, 2);


        if (state.rb == 1) {
            game->mode = one_player_chosen ? ONE_PLAYER : TWO_PLAYERS;
            _continue = 0;
        }

        redrawData(mem);
        parlcd_delay(20);
    }

    clearData();
    redrawData(mem);


    int level = 0;
    is_pressing = 0;
    _continue = 1;
    int unlock = 0;


    fillBgImg("/tmp/mozguana/logo.ppm");
    drawString("LEVEL", 5, 12, ORANGE, 0x0, 2);

    while (_continue) {
        Knobs_state state = getKnobsValue();

        if (state.gb == 1 && !is_pressing) {
            level = (level + 1) % 3;
            is_pressing = 1;
        } else if (!state.gb) {
            is_pressing = 0;
        }

        // Fill the background of current selection with blue
        drawString("EASY", 7, 4, 0xFFFF, level == 0 ? BLUE : 0x0, 2);
        drawString("MEDIUM", 7, 12, 0xFFFF, level == 1 ? BLUE : 0x0, 2);
        drawString("HARD", 7, 22, 0xFFFF, level == 2 ? BLUE : 0x0, 2);

        redrawData(mem);
        parlcd_delay(20);

        // Wait while red button won't be pressed
        if (!unlock && !state.rb) unlock = 1;

        if (state.rb && unlock) {
            game->currentPlayer->level = level % 3;
            _continue = 0;
        }
    }
}

// Print score in tne score area if the screen
void printScore(int score, char buffer[], int bufferSize) {
    snprintf(buffer, bufferSize, "SCORE: %d", score);
    drawString(buffer, 3, 25, 0xFFFF, 0x0, 1);
    drawString("Next:", 6, 26, 0xFFFF, 0x0, 1);
}


void playGame(Game *game) {
    sleep(1);
    clock_t before = clock();

    Knobs_state state = getKnobsValue();
    Knobs_state new_state = getKnobsValue();

    int success = 1;
    int _continue = 1;
    char buffer[100];
    // Chose a period of figure falling in ms depending on chosen level(easy-900ms, normal- 700ms, difficult- 550ms)
    int period = !game->currentPlayer->level ? 900 : (game->currentPlayer->level == 1 ? 700 : 550);

    // If two players, wait for the opponent, draw logo and loading animation
    if (game->mode == TWO_PLAYERS) {
        clearData();
        fillBgImg("/tmp/mozguana/logo.ppm");
        drawString("WAITING FOR THE OPPONENT", 6, 4, 0xFFFF, 0x0, 2);
        redrawData(mem);
        int quantity = 0;

        while (game->currentPlayer->status != GAME_IN_PROGRESS) {
            drawString("      ", 7, 13, 0xFFFF, 0x0, 2);
            for (int i = 0; i < quantity; i++) {
                drawString(". ", 7, 13 + i, 0xFFFF, 0x0, 2);
            }
            quantity = (quantity + 1) % 4;
            redrawData(mem);
            sleep(1);
        };
    }
    clearData();

    game->currentFigure = initRandomFigure();
    game->nextFigure = initRandomFigure();

    addFigureToField(game->currentFigure, game->currentPlayer->game_field, 0);
    addFigureToField(game->nextFigure, game->currentPlayer->game_field, 1);
    printScore(game->currentPlayer->score, buffer, sizeof buffer);

    while (_continue) {
        if (new_state.gk - state.gk >= 4) {
            moveFigure(1, 0, game->currentFigure, game->currentPlayer);
            state.gk = new_state.gk;
        } else if (state.gk - new_state.gk >= 4) {
            moveFigure(-1, 0, game->currentFigure, game->currentPlayer);
            state.gk = new_state.gk;
        }

        if (new_state.rk - state.rk >= 4 || state.rk - new_state.rk >= 4) {
            rotateFigure(game->currentFigure, new_state.rk < state.rk, game->currentPlayer->game_field);
            state.rk = new_state.rk;
        }

        if (new_state.gb) {
            success = moveFigure(0, 1, game->currentFigure, game->currentPlayer);
        }


        clock_t diff = clock() - before;
        int mces = diff * 1000 / CLOCKS_PER_SEC;

        if (mces > period && success) {
            success = moveFigure(0, 1, game->currentFigure, game->currentPlayer);
            before = clock();
        }

        new_state = getKnobsValue();
        redraw(mem, game->currentPlayer->game_field, game->mode == TWO_PLAYERS ? game->opponent->game_field : NULL);

        if (!success) {
            clearData();
            Figure *new_figure = initRandomFigure();

            memcpy(game->currentFigure, game->nextFigure, sizeof(Figure));
            memcpy(game->nextFigure, new_figure, sizeof(Figure));

            addFigureToField(game->currentFigure, game->currentPlayer->game_field, 0);
            addFigureToField(game->nextFigure, game->currentPlayer->game_field, 1);

            free(new_figure);
            success = 1;

            printScore(game->currentPlayer->score, buffer, sizeof buffer);
            if (game->currentPlayer->score >= 1000 && !(game->currentPlayer->score % 1000)) {
                period -= 10;
            }

            if (willCollide(game->currentFigure, 0, 1, game->currentPlayer->game_field)) {
                _continue = 0;
            }

        }

        if (game->mode == TWO_PLAYERS && !game->opponent->game_field) {
            char buffer[100];
            snprintf(buffer, sizeof buffer, "SCORE IS %d", game->opponent->score);

            drawString("END OF THE GAME!", 5, 40, 0xFFFF, 0x0, 1);
            drawString("YOUR OPPONENT'S ", 8, 40, 0xFFFF, 0x0, 1);
            drawString(buffer, 9, 43, 0xFFFF, 0x0, 1);
        }

        parlcd_delay(16);
    }

    game->currentPlayer->status = GAME_END;

    snprintf(buffer, sizeof buffer, "YOUR SCORE IS %d", game->currentPlayer->score);

    clearData();
    drawString("END OF THE GAME!", 7, 8, 0xFFFF, 0x0, 1);
    drawString(buffer, 10, 8, 0xFFFF, 0x0, 1);
    redrawData(mem);

    if (game->mode == TWO_PLAYERS) {
        while (game->opponent->game_field) {
            redraw(mem, NULL, game->opponent->game_field);
            parlcd_delay(16);
        }
        clearData();
        sleep(1);

        drawString(buffer, 8, 15, 0xFFFF, 0x0, 1);
        snprintf(buffer, sizeof buffer, "YOUR OPPONENT'S SCORE IS %d", game->opponent->score);
        drawString(buffer, 10, 14, 0xFFFF, 0x0, 1);

        if (game->opponent->score > game->currentPlayer->score) {
            drawString("YOU LOSE!", 6, 8, 0xFFFF, 0x0, 2);
        } else if (game->opponent->score < game->currentPlayer->score) {
            drawString("YOU WON!", 6, 8, 0xFFFF, 0x0, 2);
        } else {
            drawString("DRAW!", 6, 8, 0xFFFF, 0x0, 2);
        }
        redrawData(mem);
    }

    showRatingTable(game->currentPlayer);
    redrawData(mem);

    return NULL;
}

void freeGame(Game *game){
    free(game->currentFigure);
    free(game->nextFigure);
    freePlayer(game->currentFigure);
    if (game->mode==TWO_PLAYERS){
        freePlayer(game->opponent);
    }
    free(game);
}