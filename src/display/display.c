
#include "display.h"

uint16_t *data;

/*
 * Initialization of array of uint16_t colors for storing display information
 */
void initData() {
    data = (uint16_t *) calloc(HEIGHT * WIDTH, sizeof(uint16_t));
}

unsigned char *initDisplay() {
    unsigned char *parlcd_mem_base;

    parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

    // if (parlcd_mem_base == NULL)  exit(1);
    parlcd_hx8357_init(parlcd_mem_base);
    return parlcd_mem_base;
}

/*
 * Redraw display while playing, when display is separated:
 * - in two parts in case of two players(player field and additional data like score and next figure),
 * - in three parts in case of two player mode
 */
void redraw(unsigned char *parlcd_mem_base, Cell *playerField, Cell *opponentField) {
    parlcd_write_cmd(parlcd_mem_base, 0x2c);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j < GAME_FIELD_WIDTH * CELL_SIZE && playerField) {
                //draw game field of player if exists
                parlcd_write_data(parlcd_mem_base, getCell(playerField, i / CELL_SIZE, j / CELL_SIZE)->color);
            } else if (j == GAME_FIELD_WIDTH * CELL_SIZE && playerField) {
                // draw the border of game field if exists
                parlcd_write_data(parlcd_mem_base, 0xFFFF);
            } else if (opponentField && j >= GAME_FIELD_WIDTH * 2 * CELL_SIZE) {
                // draw opponent field if exists
                Cell *cell = getCell(opponentField, i / CELL_SIZE - 2, j / CELL_SIZE);
                parlcd_write_data(parlcd_mem_base, cell->state ? cell->color : 0x0);
            } else if (opponentField && j == GAME_FIELD_WIDTH * 2 * CELL_SIZE - 1) {
                // draw border of opponent field if exists
                parlcd_write_data(parlcd_mem_base, 0xFFFF);
            } else {
                // draw additional data
                parlcd_write_data(parlcd_mem_base, data[i * WIDTH + j]);
            }
        }
    }
}

// redraw display while the game is not started
void redrawData(unsigned char *parlcd_mem_base) {
    parlcd_write_cmd(parlcd_mem_base, 0x2c);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            parlcd_write_data(parlcd_mem_base, data[i * WIDTH + j]);
        }
    }
}

// fill the data of the size of 1 cell
void fillCell(int row, int col, uint16_t color) {
    for (int i = row * CELL_SIZE; i < (row + 1) * CELL_SIZE; i++) {
        for (int j = col * CELL_SIZE; j < (col + 1) * CELL_SIZE; j++) {
            data[i * WIDTH + j] = color;
        }
    }
}

/* draw one letter to the screen,
* @param letter - letter to draw
* @param row
* @param col
* @param color- color of the character
* @param background - background of the character
* @param scale - scale letter ${scale} times
*/
void drawLetter(char letter, int row, int col, uint16_t color, uint16_t bg, int scale) {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 8; j++) {
            // Scaling font size
            for (int m = 0; m < scale; m++) {
                for (int n = 0; n < scale; n++) {
                    data[((i + row * 16) * scale + m) * WIDTH + (col * 8 + j) * scale + n] =
                            (font_rom8x16.bits[(int) letter * 16 + i] >> (15 - j)) & 1 ? color : bg;
                }
            }
        }
    }
}

/* draw string to the screen,
* @param str - string to draw
* @param row
* @param col
* @param color- color of the string
* @param background - background of the string
* @param scale - scale every letter in ${str} ${scale} times
*/
void drawString(char *str, int row, int col, int16_t color, uint16_t bg, int scale) {
    for (int i = 0; i < strlen(str); i++) {
        drawLetter(str[i], row, col + i, color, bg, scale);
    }
}

/*
 * Fills background with image ppm format in the center of the screen width
 * @param filename -  name of the file
 */

void fillBgImg(char *filename) {
    FILE *file = fopen(filename, "r");

    int rows = 320;
    int cols = 480;
    int max_intensity = 0;

    fscanf(file, "P6 %d %d %d\n", &cols, &rows, &max_intensity);

    uint8_t *img = (uint8_t *) calloc(sizeof(uint8_t), rows * cols * 3);

    fread(img, sizeof(uint8_t), rows * cols * 3, file);

    fclose(file);

    int i, j;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            uint16_t color = ((img[(cols * i + j) * 3] & 0xf8) << 8) | ((img[(cols * i + j) * 3 + 1] & 0xfc) << 3) |
                             (img[(cols * i + j) * 3 + 2] >> 3);
            data[WIDTH * i + j + (WIDTH - cols) / 2] = color;
        }
    }
    free(img);
}

void clearData() {
    memset(data, 0, sizeof(uint16_t) * HEIGHT * WIDTH);
}

void freeData() {
    free(data);
}