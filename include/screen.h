#ifndef SCREEN_H
#define SCREEN_H

typedef unsigned char Color;
typedef struct _pixel
{
    char texture;
    Color fore_color;
    Color back_color;
}Pixel;


#define PRETO 0
#define VERMELHO 1
#define VERDE 2
#define AMARELO 3
#define AZUL 4
#define MAGENTA 5
#define CIANO 6
#define CINZA_CLARO 7
#define CINZA 8
#define VERMELHO_CLARO 9
#define VERDE_CLARO 10
#define AMARELO_CLARO 11
#define AZUL_CLARO 12
#define MAGENTA_CLARO 13
#define CIANO_CLARO 14
#define BRANCO 15

int screen_init();

void set_pixel(int x, int y, Pixel p);

void clear_scr();

void set_clear_color(Color fore, Color back);

int screen_delete();

void update_screen();

void printscr();

#endif