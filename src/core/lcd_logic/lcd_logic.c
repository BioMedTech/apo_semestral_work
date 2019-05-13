
#include "lcd_logic.h"

uint16_t *data;


void initData(){
    data=(uint16_t *) calloc (HEIGHT * WIDTH, sizeof(uint16_t));
    // for (int i=0; i<HEIGHT; i++){
    //     data[i]=(uint16_t *) calloc (WIDTH, sizeof(uint16_t));
    // }
}

unsigned char *initDisplay(){
    unsigned char *parlcd_mem_base;

    parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

    // if (parlcd_mem_base == NULL)  exit(1);
    parlcd_hx8357_init(parlcd_mem_base);
    return parlcd_mem_base;
}


void redraw(unsigned char *parlcd_mem_base, Cell **playerField){
    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    
    int n, m;
    uint16_t color;
    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            if (j < 15 * CELL_SIZE){
                n = i / CELL_SIZE;
                m = j / CELL_SIZE;
                color = playerField[n][m].state == 0 ? 0x0000 : playerField[n][m].color;
                parlcd_write_data(parlcd_mem_base, color);
            } else {
                parlcd_write_data(parlcd_mem_base, 0xFFFF);
            }
        }
    }
}

void redrawData(unsigned char *parlcd_mem_base){
    parlcd_write_cmd(parlcd_mem_base, 0x2c);

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            parlcd_write_data(parlcd_mem_base, data[i*WIDTH+j]);
        }
    }
}

void fillCell(int row, int col, uint16_t color)
{
    uint16_t invertedColor = 0xfff;

    for (int i = row * CELL_SIZE; i < (row + 1) * CELL_SIZE; i++)
    {
        for (int j = col * CELL_SIZE; j < (col + 1) * CELL_SIZE; j++)
        {
            // if (i == row * CELL_SIZE || i + 1 == (row + 1) * CELL_SIZE ||
            //     j == col * CELL_SIZE || j + 1 == (col + 1) * CELL_SIZE)
            // {
            //     data[i * WIDTH + j] = invertedColor;
            // }
            // else
            // {
                data[i * WIDTH + j] = color;
            // }
        }
    }
}

void drawLetter(char letter, int row, int column, uint16_t color, uint16_t bg)
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            data[(i + row * 16) * WIDTH + column * 8 + j] = (font_rom8x16.bits[(int)letter * 16 + i] >> (15 - j)) & 1 ? color : bg;
        }
    }
}

void drawString(char *str, int row, int column, int16_t color, uint16_t bg)
{
    for (int i=0; i<strlen(str); i++){
        drawLetter(str[i], row, column+i, color, bg);
    }
}

void fillBgImg(char *filename)
{
    printf("Hello w\n");
    FILE *file = fopen(filename, "r");
    

    int rows = 320;
    int cols = 480;
    int max_intensity = 0;

    // fscanf(file, "P6 %d %d %d\n", &cols, &rows, &max_intensity);

    uint16_t *img= (uint16_t *)calloc(sizeof(uint16_t), rows * cols);

    fread(img, sizeof(uint16_t), rows * cols, file);

    fclose(file);
    
    for (int i=0; i < rows && i < HEIGHT; i++){
        for (int j = 0; j < cols && j < WIDTH; j++){
            data[HEIGHT * i + j] = img[cols * i + j];
        }
    }
    free(img);

}

void clearData()
{
    memset(data, 0, sizeof(uint16_t) * HEIGHT * WIDTH);
} 

void freeData()
{
    free(data);
}