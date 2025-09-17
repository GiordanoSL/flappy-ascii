#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "screen.h"

void copy_color(char * DEST, Color fore, Color back);

const char * fore_black = "\033[30;";
const char * fore_red = "\033[31;";
const char * fore_green = "\033[32;";
const char * fore_yellow = "\033[33;";
const char * fore_blue = "\033[34;";
const char * fore_magenta = "\033[35;";
const char * fore_cyan = "\033[36;";
const char * fore_b_grey = "\033[37;";

const char * fore_grey = "\033[90;";
const char * fore_b_red = "\033[91;";
const char * fore_b_green = "\033[92;";
const char * fore_b_yellow = "\033[93;";
const char * fore_b_blue = "\033[94;";
const char * fore_b_magenta = "\033[95;";
const char * fore_b_cyan = "\033[96;";
const char * fore_white = "\033[97;";

const char * back_black = "40m";
const char * back_red = "41m";
const char * back_green = "42m";
const char * back_yellow = "43m";
const char * back_blue = "44m";
const char * back_magenta = "45m";
const char * back_cyan = "46m";
const char * back_white = "47m";

#define SCR_WIDTH 30
#define SCR_HEIGHT 10

static int running = 0;

static char * screen = NULL;

static char clear_color[8];

int screen_init(){
    if(running) return 1; // if an instance is already running, you cant create another one

    if(!(screen = (char *) malloc(((SCR_WIDTH * 9 + 1) * SCR_HEIGHT + 1) * sizeof(char)))){
        printf("Couldn't allocate memory for the screen ---\n");
        return -1;
    }

    printf("\033[?25l"); // Set cursor to invisible

    running = 1;

    set_clear_color(VERMELHO);
    clear_scr();

    return 0; // Initialization successful
}

int screen_delete(){
    if(!running) return 1; // If there is no instance running, you cannot delete the screen

    printf("\033[H\033[J"); // Clear the screen
    printf("\033[?25h");    // Set cursor to visible
    fflush(0);

    running = 0;

    free(screen);
    screen = NULL;
    
    return 0;
}

void clear_scr(){
    int i, j;
    char clr[9];
    
    strcpy(clr, clear_color);
    clr[8] = ' ';

    for (i = 0;i < SCR_HEIGHT;i++){
        for (j = 0; j < SCR_WIDTH * 9; j+=9)
            memcpy(&screen[j + (SCR_WIDTH * 9 + 1) * i], clr, 9);

        screen[(SCR_WIDTH * 9) * (i + 1) + i] = '\n';   
    }
    screen[((SCR_WIDTH * 9 + 1) * SCR_HEIGHT)] = '\0';
}

void set_clear_color(Color c){
    copy_color(clear_color, BRANCO, c);

}

void printscr(){
    printf("\033[H\033[J");
    printf("%s", screen);
    printf("\033[0m");
    fflush(0);
}

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
    default:
        memcpy(DEST, fore_white, 5);
        break;
    }

    back %= 8; // there are only 8 available colors for the background

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