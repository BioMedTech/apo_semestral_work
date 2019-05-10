//
// Created by ananastia on 5/4/19.
//

#include "Figure.h"

static int figures[]={
    /*
    
    Kube 
    **
    **

    */
   -1, -1,  0, -1,  -1, 0, -1, -1,
   /*
    
    L
    *
    *
    ** 
    
    */
    
    0, -1,  0, 0,  0, 1,  1, 1,
    /*
    T
     *
    ***

    */
    0, -1,  -1, 0,  0, 0,  1, 0,
    /*
     I
    ****
    */
    -1, 0, 0, 0, 1, 0, 2, 0
};

void initRandomFigure(){
   int random_val=random() % FIGURE_TYPES_QUANTITY;

   Figure *figure=(Figure *) calloc (1, sizeof(Figure));
   figure->type = random_val;
   figure->offset.x = 7;
   figure->offset.y = 7;

   figure->color = 0xF450;

   for (int i=figure->type*8, n=0; i<(figure->type+1)*8; i+=2, n++) {
     figure->state[n].x=figures[i];
     figure->state[n].y=figures[i+1];
   }

   drawFigure(figure);

}

// int inCoord(Coords* a, Coords *b[4]){
//     int in=0;
//     for (int i=0; i<4; i++){
//         if (a.x==b[i].x && a.y==b[i].y){
//             in=1;
//         }
//     }
//     return in;
// }
void drawFigure(Figure *figure){
    for (int i=0; i < FIGURE_CELL_QUANTITY; i++){
        printf("x: %d, y: %d \n", figure->offset.x + figure->state[i].x, figure->offset.y + figure->state[i].y);
        fillCell(figure->offset.x + figure->state[i].x, figure->offset.y + figure->state[i].y, figure->color);
    }
}

int moveFigure(int vector_x, int vector_y, Figure* figure){
    int prev_offset_x= figure->offset.x;
    int prev_offset_y= figure->offset.y;

    figure->offset.x += vector_x;
    figure->offset.y += vector_y;

    for (int i=0; i < 4; i++){
        fillCell(figure->state[i].y+prev_offset_y, figure->state[i].x+prev_offset_x, 0x0000);
        fillCell(figure->offset.y + figure->state[i].y, figure->offset.x + figure->state[i].x, figure->color);
    }

    return 1;
}

void rotateFigure(Figure *figure){
    
    //rotation matrix
    //(cos 90 -sin 90)=(0 -1)(x)=(-y)
    //(sin 90  cos 90) (1  0)(y) ( x)

    if (figure->type != 0){
        for (int i=0; i<4; i++){
            fillCell(figure->state[i].y + figure->offset.y, figure->state[i].x + figure->offset.x, 0x0000);
            int x = figure->state[i].x;
            figure->state[i].x = -figure->state[i].y;
            figure->state[i].y = x;
            fillCell(figure->state[i].y + figure->offset.y, figure->state[i].x + figure->offset.x, figure->color);
        }
    }
}
