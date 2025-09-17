#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "keyboard.h"
#include "screen.h"

volatile int running;

int posx = 0, posy = 0;

void draw_flappy(int x, int y){

    printf("\x1b[0m\x1b[%d;%dH", y, x*2);
    printf("\x1b[48;5;6m\x1b[38;5;0m");
    printf("    _   ");
    printf("\x1b[48;5;0m\x1b[%d;%dH", y + 1, x*2);
    printf("\x1b[48;5;6m __\x1b[48;5;11m(\x1b[48;5;7m@\x1b[48;5;11m)\x1b[48;5;6m\x1b[38;5;9m=");
    printf("\x1b[48;5;6m\x1b[38;5;0m");
    printf(" ");
    printf("\x1b[48;5;0m\x1b[%d;%dH", y + 2, x*2);
    printf("\x1b[48;5;6m \\\x1b[48;5;11m_/_)");
    printf("\x1b[48;5;6m\x1b[38;5;0m");
    printf("  ");
    printf("\x1b[?25l");

}

void draw_screen(){
    printf("\033[1J");
    for (int i = 0; i < 48; i++)
    {
        printf("\x1b[0m\x1b[%d;0H", i);
        printf("\x1b[48;5;6m\x1b[38;5;0m                                                                                                                                \n");
    
    }
    
}

void my_handler(char c) {
    if(c == KEY_BACKSPACE){
        printf("\033[1D\033[0K");
        return;
    }
    switch (c)
    {
    case KEY_UP:
        posy = (posy > 0)?posy-1:posy;
        break;
    case KEY_DOWN:
        posy = (posy < 48)?posy+1:posy;
        break;
    case KEY_RIGHT:
        posx = (posx < 64)?posx+1:posx;
        break;
    case KEY_LEFT:
        posx = (posx > 0)?posx-1:posx;
        break;
    case KEY_0:
        set_clear_color(PRETO);
        break;
    case KEY_1:
        set_clear_color(VERMELHO);
        break;
    case KEY_2:
        set_clear_color(VERDE);
        break;
    case KEY_3:
        set_clear_color(AMARELO);
        break;
    case KEY_4:
        set_clear_color(AZUL);
        break;
    case KEY_5:
        set_clear_color(MAGENTA);
        break;
    case KEY_6:
        set_clear_color(CIANO);
        break;
    case KEY_7:
        set_clear_color(BRANCO);
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
    if(screen_init()) exit(0);

    printscr();

    while (running){
        usleep(100000);
        clear_scr();
        printscr();
    }
    
    
    // draw_screen();
    // draw_flappy(posx, posy);
    
    keyboard_shutdown();
    screen_delete();
    return 0;
}

/*
              @@@@@@@@@@             
            @@@:::::::@@.-@              
          @@::::::::@@.....@@          
      #@@@@@@:::::::@@...-@..@@        
     @%......@@:::::@@...-@..@@        
     @%........@@:::::@@.....@@        
     @%:.....::@@:::::::@@@@@@@@       
      #@:::::@@:::::::@@========@@     
        @@@@@:::::::@@==@@@@@@@@       
            @:::::::::@@=======@       
             @@@@:::::::@@@@@@@@       
                 @@@@@@@               

      
   _ 
__(.)>
\_/_)    

                                                
*/
