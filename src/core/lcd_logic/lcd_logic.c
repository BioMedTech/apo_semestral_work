
#include "lcd_logic.h"

uint16_t **data;


void initData(){
    data=(uint16_t **) calloc (HEIGHT, sizeof(uint16_t *));
    for (int i=0; i<HEIGHT; i++){
        data[i]=(uint16_t *) calloc (WIDTH, sizeof(uint16_t));
    }
    // return data;
}

unsigned char *initDisplay(){
    unsigned char *parlcd_mem_base;

    parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

    // if (parlcd_mem_base == NULL)  exit(1);
    parlcd_hx8357_init(parlcd_mem_base);
    return parlcd_mem_base;
}


void redraw(unsigned char *parlcd_mem_base){
        parlcd_write_cmd(parlcd_mem_base, 0x2c);
        
        for (int i=0; i < HEIGHT; i++){
            for (int j = 0; j < WIDTH; j++){
                parlcd_write_data(parlcd_mem_base, data[i][j]);
            }
        }
}


void fillCell(int row, int col, uint16_t color){
    for (int i = row * CELL_SIZE; i < (row + 1)*CELL_SIZE; i++){
        for (int j = col * CELL_SIZE; j < (col + 1)* CELL_SIZE; j++) {
            data[i][j] = color;
        }
    }
}

int checkCell(int row, int col){

}

void draw_letter(char letter, int row, int column, uint16_t color, uint16_t bg){
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 8; j++){
            data[i+row*16][column*8 + j] = (font_rom8x16.bits[(int) letter * 16 + i] >> (15 - j)) & 1 ? color : bg;
        }
    }
}


void freeData(){
    for (int i=0; i<HEIGHT; i++){
        free(data[i]);
    }
    free(data);
}