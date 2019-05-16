//
// Created by ananastia on 5/4/19.
//

#include "Figure.h"

static uint16_t colors[] = {
    //red
    0xF800,
    //green
    0x07E0,
    //blue
    0x001F,
    //lightblue
    0x07FF,
    //yellow
    0xFFE0
};

static int figures[] = {
    /*
    
    Kube 
    **
    **

    */
    -1, -1, 0, -1, -1, 0, 0, 0,
    /*
    
    L
    *
    *
    ** 
    
    */

    0, -1, 0, 0, 0, 1, 1, 1,
    /*
    T
     *
    ***

    */
    0, -1, -1, 0, 0, 0, 1, 0,
    /*
     I
    ****
    */
    -1, 0, 0, 0, 1, 0, 2, 0
    };

Figure *initRandomFigure(Cell **gameField)
{
    Figure *figure = (Figure *)calloc(1, sizeof(Figure));
    figure->type = random() % FIGURE_TYPES_QUANTITY;
    figure->offset.x = 5;
    figure->offset.y = 1;

    figure->color = colors[random() % 5];

    for (int i = figure->type * 8, n = 0; i < (figure->type + 1) * 8; i += 2, n++)
    {
        figure->state[n].x = figures[i];
        figure->state[n].y = figures[i + 1];
        
        gameField[figures[i + 1] + figure->offset.y][figures[i] + figure->offset.x].color = figure->color;
        gameField[figures[i + 1] + figure->offset.y][figures[i] + figure->offset.x].state = 2;
    }

    return figure;
}

int moveFigure(int vector_x, int vector_y, Figure *figure, Player *player)
{
    // printf("Moving figure to vector x: %d, y: %d\n", vector_x, vector_y);

    int success = 1;
    int x, y;

    Cell **gameField=player->game_field;

    int collision = willCollide(figure, vector_x, vector_y, gameField);

    if (!collision)
    {
        for (int i = 0; i < FIGURE_CELL_QUANTITY; i++)
        {
            x = figure->offset.x + figure->state[i].x;
            y = figure->offset.y + figure->state[i].y;

            // if (gameField[y][x].state == 2){
                gameField[y][x].state = 0;
            // }
        
        }

        figure->offset.x += vector_x;
        figure->offset.y += vector_y;
        
        for (int i = 0; i < FIGURE_CELL_QUANTITY; i++)
        {
            x = figure->offset.x + figure->state[i].x;
            y = figure->offset.y + figure->state[i].y;

            // if (gameField[y][x].state == 0)
            // {
                gameField[y][x].state = 2;
                gameField[y][x].color = figure->color;
            // }
        }
    } else if (collision == 1 && vector_y){
        success = 0;
        changePlayerField(figure, player, 1);
    }
    
    return success;
}

void rotateFigure(Figure *figure, int clock, Cell **gameField)
{
    //rotation matrix 90 degrees
    //(cos 90 -sin 90)=(0 -1)(x)=(-y)
    //(sin 90  cos 90) (1  0)(y) ( x)
    
    //rotation matrix -90 degrees
    //(cos -90 -sin -90)=( 0  1)(x)=( y)
    //(sin -90  cos -90) (-1  0)(y) (-x)

    if (figure->type != 0){
        Coords testCoords[4];
        memcpy(testCoords, figure->state, sizeof(Coords) * 4);
        
        int x_mult = clock ? -1 : 1;
        int y_mult = clock ? 1 : -1;
    
        int x, y, x1, collision=0;

        for (int i = 0; i < FIGURE_CELL_QUANTITY; i++)
        {
            x1 = testCoords[i].x * x_mult;
            testCoords[i].x = y_mult * testCoords[i].y;
            testCoords[i].y = x1;

            x = testCoords[i].x + figure->offset.x;
            y = testCoords[i].y + figure->offset.y;

        if (y >= GAME_FIELD_HEIGHT || y<0 || x < 0 || x >= GAME_FIELD_WIDTH || gameField[y][x].state == 1)
             collision = 1;
        }

        if (!collision){
            for (int i = 0; i < FIGURE_CELL_QUANTITY; i++)
            {
                gameField[figure->state[i].y + figure->offset.y][figure->state[i].x + figure->offset.x].state = 0;   
            }

            memcpy(figure->state, testCoords, sizeof(Coords) * 4);

            for (int i = 0; i < FIGURE_CELL_QUANTITY; i++)
            {
                x = figure->state[i].x + figure->offset.x;
                y = figure->state[i].y + figure->offset.y;
                
                gameField[y][x].state = 2;
                gameField[y][x].color = figure->color;
            }   
        }
        
    }
}

int checkFullRow(int row, Cell **gameField){
    for (int i = 0; i < GAME_FIELD_WIDTH; i++){
        if (!gameField[row][i].state){
            return 0;
        }
    }
    return 1;
}

int removeRow(int row, Cell **gameField){
    int count=1;
    for (int i = row; i > 1; i--){
        memcpy(gameField[i], gameField[i-1], sizeof(Cell) * GAME_FIELD_WIDTH);
    }
    for (int i=0; i < GAME_FIELD_WIDTH; i++){
        gameField[0][i].state = 0;
    }
    if (checkFullRow(row, gameField))
    {
        removeRow(row, gameField);
        count++;
    }
    return count;
}


void changePlayerField(Figure *figure, Player *player, int state)
{
    int x, y;
    Cell **gameField=player->game_field;
    
    for (int i = 0; i < FIGURE_CELL_QUANTITY; i++)
    {
        x = figure->state[i].x + figure->offset.x;
        y = figure->state[i].y + figure->offset.y;
       
        gameField[y][x].state = state;
        gameField[y][x].color = figure->color;
    }
    int count = 0;

    for (int i = 0; i < FIGURE_CELL_QUANTITY; i++)
    {
        y = figure->state[i].y + figure->offset.y;
        
        if (checkFullRow(y, gameField))
        {
            count+=removeRow(y, gameField);
        }
    }
    if (count) {
        player->score += 600 * (player->level + 1)/(4-count);
        setLedValues(0xFF0000);
        setLedValues(0x0);
    }
}

int willCollide(Figure *figure, int vector_x, int vector_y, Cell **gameField)
{
    int collision = 0;
    int x, y;
    for (int i = 0; i < FIGURE_CELL_QUANTITY; i++)
    {
        x = figure->state[i].x + figure->offset.x + vector_x;
        y = figure->state[i].y + figure->offset.y + vector_y;

        if (x < 0 || x >= 15 || y >= 20 || gameField[y][x].state == 1)
        {
            collision = 1;
        } 
    }

    return collision;
}