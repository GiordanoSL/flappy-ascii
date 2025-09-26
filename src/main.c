#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "keyboard.h"
#include "screen.h"
#include "bird.h"
#include "pipes.h"

volatile int running;

Bird * flappy;

void my_handler(char c) {
    if(c == KEY_BACKSPACE){
        printf("\033[1D\033[0K");
        return;
    }
    switch (c){
    case KEY_0:
        set_clear_color(PRETO,PRETO);
        break;
    case KEY_1:
        set_clear_color(VERMELHO,VERMELHO);
        break;
    case KEY_2:
        set_clear_color(VERDE,VERDE);
        break;
    case KEY_3:
        set_clear_color(AMARELO,AMARELO);
        break;
    case KEY_4:
        set_clear_color(AZUL,AZUL);
        break;
    case KEY_5:
        set_clear_color(MAGENTA,MAGENTA);
        break;
    case KEY_6:
        set_clear_color(CIANO,CIANO);
        break;
    case KEY_7:
        set_clear_color(BRANCO,BRANCO);
        break;
    case KEY_SPACE:
        bird_flap(flappy);
        break;
    case KEY_ENTER:
        bird_respawn(flappy);
        break;
    
    default:
        break;
    }
    if (c == 'q') {
        printf("Exiting...\n");
        running = 0;
    }
}


int main() {

    running = 1;

    keyboard_init();
    keyboard_set_handler(my_handler);

    set_clear_color(CIANO, CIANO);
    if(screen_init()) exit(0);

    flappy = bird_create();
    printscr();

    int i = 0, y;
    while (running){
        usleep(16666);

        if(i%10 == 0){
            y = bird_get_pos(flappy);
            if (y >= SCR_HEIGHT - 4)
                bird_kill(flappy);
            bird_update(flappy);
            pipes_update();
            if(!i)
                pipe_create();
        }
        clear_screen_buffer();
        update_screen();
        draw_rect(0, SCR_HEIGHT - 2, SCR_WIDTH, 2, '#', VERDE_CLARO, VERDE, true);
        update_screen();
        bird_draw(flappy);
        update_screen();
        pipes_draw();
        update_screen();
        printscr();
        i++;
        i%= 150;
    }
    
    keyboard_shutdown();
    screen_delete();
    return 0;
}

