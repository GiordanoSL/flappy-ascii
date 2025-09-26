#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "screen.h"

void copy_color(char * DEST, Color fore, Color back);

// Escape sequences for colors

// Foreground regular colors
const char * fore_black = "\033[30;";
const char * fore_red = "\033[31;";
const char * fore_green = "\033[32;";
const char * fore_yellow = "\033[33;";
const char * fore_blue = "\033[34;";
const char * fore_magenta = "\033[35;";
const char * fore_cyan = "\033[36;";
const char * fore_b_grey = "\033[37;";
// Foreground bright colors
const char * fore_grey = "\033[90;";
const char * fore_b_red = "\033[91;";
const char * fore_b_green = "\033[92;";
const char * fore_b_yellow = "\033[93;";
const char * fore_b_blue = "\033[94;";
const char * fore_b_magenta = "\033[95;";
const char * fore_b_cyan = "\033[96;";
const char * fore_white = "\033[97;";

// Background colors 
const char * back_black = "40m";
const char * back_red = "41m";
const char * back_green = "42m";
const char * back_yellow = "43m";
const char * back_blue = "44m";
const char * back_magenta = "45m";
const char * back_cyan = "46m";
const char * back_white = "47m";

static int running = 0;

static char * screen = NULL;

static Color clear_color_fore = BRANCO;
static Color clear_color_back = PRETO;

static Pixel screen_buffer[SCR_HEIGHT][SCR_WIDTH];

int screen_init() {
    if (running) return 1; // Cannot create a new instance if one is already running

    // Each pixel in 'screen_buffer' expands into 9 bytes in 'screen' (8 for colors + 1 for texture)
    // Each row in 'screen' has an extra '\n' at the end (SCR_WIDTH * 9 + 1)
    // The total size includes SCR_HEIGHT rows plus 1 byte for the null terminator
    if (!(screen = (char *) malloc(((SCR_WIDTH * 9 + 1) * SCR_HEIGHT + 1) * sizeof(char)))) {
        perror("Couldn't allocate memory for the screen ---\n");
        exit(-1);
    }

    // Hide the cursor while the screen is active
    printf("\033[?25l");

    running = 1;

    // Fill the screen buffer
    clear_screen_buffer();
    update_screen();

    return 0; // Initialization successful
}

int screen_delete(){
    if(!running) return 1; // Cannot delete if there is no instance running

    // Clear the terminal screen and move the cursor to home position
    printf("\033[H\033[J");

    // Make cursor visible again
    printf("\033[?25h");

    // Ensure changes are applied imediately
    fflush(0);

    // Mark the screen as no longer running 
    running = 0;
    
    // Free the memory used for the screen array
    free(screen);
    screen = NULL;
    
    return 0;
}

void update_screen(){
    if(!running) return;

    int i, j;

    for (i = 0; i < SCR_HEIGHT; i++){
        for (j = 0; j < SCR_WIDTH; j++){
            // Write 8-byte color escape codes + 1-byte texture into the linear screen array
            copy_color(&screen[j * 9 + (SCR_WIDTH * 9 + 1) * i], 
                        screen_buffer[i][j].fore_color, 
                        screen_buffer[i][j].back_color);
            screen[j * 9 + (SCR_WIDTH * 9 + 1) * i + 8] = screen_buffer[i][j].texture;
        }
        // Add newline at the end of each row
        screen[(SCR_WIDTH * 9) * (i + 1) + i] = '\n';   
    }
    // Null-terminate the screen array
    screen[((SCR_WIDTH * 9 + 1) * SCR_HEIGHT)] = '\0';
}

void set_pixel(int pos_x, int pos_y, char texture, Color fore, Color back){
    if(!running) return;
    if(pos_x < 0 || pos_x >= SCR_WIDTH || pos_y < 0 || pos_y >= SCR_HEIGHT) return;

    screen_buffer[pos_y][pos_x].texture = texture;
    screen_buffer[pos_y][pos_x].fore_color = fore;
    screen_buffer[pos_y][pos_x].back_color = back;
}

void draw_rect(int pos_x, int pos_y, int width, int height, char texture, Color fore, Color back, bool Border){
    if (!running) return;

    for (int i = pos_x; i < pos_x + width; i++)
        for (int j = pos_y; j < pos_y + height; j++)
            set_pixel(i, j, texture, fore, back);    
    
    if(!Border) return;

    for (int i = pos_x + 1; i < pos_x + width - 1; i++){
        set_pixel(i, pos_y, '-', fore, back);
        set_pixel(i, pos_y + height - 1, '-', fore, back);
    }
    
    for (int i = pos_y + 1; i < pos_y + height - 1; i++){
        set_pixel(pos_x, i, '|', fore, back);
        set_pixel(pos_x + width - 1, i, '|', fore, back);
    }
        
    set_pixel(pos_x, pos_y, '+', fore, back);
    set_pixel(pos_x + width - 1, pos_y, '+', fore, back);
    set_pixel(pos_x, pos_y + height - 1, '+', fore, back);
    set_pixel(pos_x + width - 1, pos_y + height - 1, '+', fore, back);
}

// Draw a rectangle outline on the screen buffer.
void draw_rect_outline(int pos_x, int pos_y, int width, int height, char texture, Color fore, Color back){
    if (!running) return;
    
    for (int i = pos_x; i < pos_x + width; i++){
        set_pixel(i, pos_y, texture, fore, back);
        set_pixel(i, pos_y + height - 1, texture, fore, back);
    }
    
    for (int i = pos_y + 1; i < pos_y + height - 1; i++){
        set_pixel(pos_x, i, texture, fore, back);
        set_pixel(pos_x + width - 1, i, texture, fore, back);
    }    
}

void clear_screen_buffer(){
    if (!running) return;
    
    int i, j;
    for (i = 0; i < SCR_HEIGHT; i++){
        for (j = 0; j < SCR_WIDTH; j++){
            // Reset each pixel character to space and the clear colors
            screen_buffer[i][j].texture = ' ';
            screen_buffer[i][j].fore_color = clear_color_fore;
            screen_buffer[i][j].back_color = clear_color_back;
        }
    }
}

void set_clear_color(Color fore, Color back){
    clear_color_fore = fore;
    clear_color_back = back;
}

void printscr() {
    if(!running) return;

    // Clear the terminal screen and move the cursor to home position
    printf("\033[H\033[J");

    // Print the screen array
    printf("%s", screen);

    // Reset terminal color attributes to default
    printf("\033[0m");
    
    // Ensure all output is written immediately
    fflush(0);
}

// Write ANSI escape codes for foreground and background colors into DEST.
// DEST must have at least 8 bytes; no null terminator is added.
// Foreground defaults to white if invalid; background is reduced modulo 8 
// since there are only 8 colors available for the background instead of 16.
void copy_color(char * DEST, Color fore, Color back){

    switch (fore){
    case PRETO:
        memcpy(DEST, fore_black, 5);
        break;
    case VERMELHO:
        memcpy(DEST, fore_red, 5);
        break;
    case VERDE:
        memcpy(DEST, fore_green, 5);
        break;
    case AMARELO:
        memcpy(DEST, fore_yellow, 5);
        break;
    case AZUL:
        memcpy(DEST, fore_blue, 5);
        break;
    case MAGENTA:
        memcpy(DEST, fore_magenta, 5);
        break;
    case CIANO:
        memcpy(DEST, fore_cyan, 5);
        break;
    case CINZA_CLARO:
        memcpy(DEST, fore_b_grey, 5);
        break;
    case CINZA:
        memcpy(DEST, fore_grey, 5);
        break;
    case VERMELHO_CLARO:
        memcpy(DEST, fore_b_red, 5);
        break;
    case VERDE_CLARO:
        memcpy(DEST, fore_b_green, 5);
        break;
    case AMARELO_CLARO:
        memcpy(DEST, fore_b_yellow, 5);
        break;
    case AZUL_CLARO:
        memcpy(DEST, fore_b_blue, 5);
        break;
    case MAGENTA_CLARO:
        memcpy(DEST, fore_b_magenta, 5);
        break;
    case CIANO_CLARO:
        memcpy(DEST, fore_b_cyan, 5);
        break;
    case BRANCO:
        memcpy(DEST, fore_white, 5);
        break;
    case CLEAR:
        break;
    default:
        memcpy(DEST, fore_white, 5);
        break;
    }

    if(back == CLEAR) return;
    
    back = (Color) (back % 8); // There are only 8 available colors for the background

    switch (back){
    case PRETO:
        memcpy(DEST + 5, back_black, 3);
        break;
    case VERMELHO:
        memcpy(DEST + 5, back_red, 3);
        break;
    case VERDE:
        memcpy(DEST + 5, back_green, 3);
        break;
    case AMARELO:
        memcpy(DEST + 5, back_yellow, 3);
        break;
    case AZUL:
        memcpy(DEST + 5, back_blue, 3);
        break;
    case MAGENTA:
        memcpy(DEST + 5, back_magenta, 3);
        break;
    case CIANO:
        memcpy(DEST + 5, back_cyan, 3);
        break;
    case CINZA_CLARO:
        memcpy(DEST + 5, back_white, 3);
        break;
    default:
        memcpy(DEST + 5, back_black, 3);
        break;
    }
}