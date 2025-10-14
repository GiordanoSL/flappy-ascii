#include "screen.h"
#include "bird.h"
#include "config.h"
#include "keyboard.h"
#include "numbers.h"
#include "pipes.h"
#include "game.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_SCORE 99

typedef enum {
    GAME_INIT,
    GAME_MENU,
    GAME_RUNNING,
    GAME_PAUSED,
    GAME_WIN,
    GAME_OVER,
    GAME_EXIT
} GameState;

GameState current;

Bird * flappy;

static int score, high_score;

void menu_key_handler(char c);
void game_key_handler(char c);
void paused_key_handler(char c);
void over_key_handler(char c);

int game_init(){
    current = GAME_INIT;

    set_clear_color(CIANO, CIANO);
    if(screen_init()) exit(0);
    keyboard_init();

    flappy = bird_create();
    pipe_create();

    score = 0;

    FILE * f = fopen("high_score.bin", "rb");
    if (f != NULL){
        fread(&high_score, sizeof(int), 1, f);
        fclose(f);
    } else high_score = 0;
    

    return 0;
}

void game_restart(){
    score = 0;
    pipes_destroy();
    pipe_create();
    bird_respawn(flappy);
    current = GAME_RUNNING;
    keyboard_set_handler(game_key_handler);
}

void draw_text_center(const char * text, int len, int pos_y, Color fore, Color back){
    int pos_x = SCR_WIDTH/2 - len/2;
    for (int i = 0; i < len; i++){
        set_pixel(pos_x + i, pos_y, text[i], fore, back);
    }
    update_screen();
}

void draw_menu_text(){
    draw_text_center("PRESS ANY KEY TO START", 22, SCR_HEIGHT/2 - 10, CINZA, BRANCO);
    draw_text_center("How to play : Press space to flap your wings", 44, SCR_HEIGHT/2 - 9, CINZA, BRANCO);

    update_screen();
}

void draw_pause_text(){
    draw_text_center("PRESS SPACE TO RESUME GAME", 26, SCR_HEIGHT/2 - 3, CINZA, BRANCO);
    draw_text_center("PRESS ESC TO EXIT GAME", 22, SCR_HEIGHT/2, CINZA, BRANCO);
    draw_text_center("PRESS 'R' TO RESTART", 20, SCR_HEIGHT/2 + 3, CINZA, BRANCO);

    update_screen();
    printscr();
}

void draw_background(){
    clear_screen_buffer();
    update_screen();
    draw_rect(0, SCR_HEIGHT - 2, SCR_WIDTH, 2, '=', VERDE_CLARO, VERDE, false); // grass
    update_screen();

}

void draw_score(int SCORE_X_POS, int SCORE_Y_POS){
    int first_digit  = score / 10;
    int second_digit = score % 10;

    // Drawing first digit
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            set_pixel(SCORE_X_POS + j, SCORE_Y_POS + i, numbers[first_digit][i][j], BRANCO, CLEAR);
        }
    }
    // Drawing second digit
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            set_pixel(SCORE_X_POS + j + 3, SCORE_Y_POS + i, numbers[second_digit][i][j], BRANCO, CLEAR);
        }
    }
    update_screen();
}

void draw_high_score(int SCORE_X_POS, int SCORE_Y_POS){
    int first_digit  = high_score / 10;
    int second_digit = high_score % 10;

    draw_text_center(" HS:     ", 8, SCORE_Y_POS, BRANCO, PRETO);
    // Drawing first digit
    set_pixel(SCORE_X_POS + 2, SCORE_Y_POS, '0' + first_digit, BRANCO, PRETO);
        
    // Drawing second digit
    set_pixel(SCORE_X_POS + 3, SCORE_Y_POS, '0' + second_digit, BRANCO, PRETO);

    update_screen();
}

void draw_over(){
    draw_background();
    pipes_draw();
    bird_draw(flappy);
    draw_rect(SCR_WIDTH/2 - 6, SCR_HEIGHT/2 - 9, 12, 3, ' ', VERMELHO, PRETO, true);
    draw_text_center("GAME  OVER", 10, SCR_HEIGHT/2 - 8, VERMELHO, PRETO);
    draw_text_center("FINAL SCORE:", 12, SCR_HEIGHT/2 - 3, BRANCO, PRETO);
    draw_rect(SCR_WIDTH/2 - 6, SCR_HEIGHT/2 - 2, 12, 4, ' ', AMARELO, PRETO, true);
    draw_score(SCR_WIDTH/2 - 3, SCR_HEIGHT/2 - 2);
    draw_high_score(SCR_WIDTH/2 - 1, SCR_HEIGHT/2 + 2);
    draw_text_center("PRESS ESC TO EXIT GAME", 22, SCR_HEIGHT/2 + 5, CINZA, BRANCO);
    draw_text_center("PRESS 'R' TO RESTART", 20, SCR_HEIGHT/2 + 7, CINZA, BRANCO);
    printscr();
}

void draw_win(){
    draw_background();
    pipes_draw();
    bird_draw(flappy);
    draw_rect(SCR_WIDTH/2 - 6, SCR_HEIGHT/2 - 9, 12, 3, ' ', AMARELO, PRETO, true);
    draw_text_center("YOU  WIN", 8, SCR_HEIGHT/2 - 8, AMARELO, PRETO);
    draw_text_center("FINAL SCORE:", 12, SCR_HEIGHT/2 - 3, CINZA, PRETO);
    draw_rect(SCR_WIDTH/2 - 6, SCR_HEIGHT/2 - 2, 12, 4, ' ', AMARELO, PRETO, true);
    draw_score(SCR_WIDTH/2 - 3, SCR_HEIGHT/2 - 2);
    draw_high_score(SCR_WIDTH/2 - 1, SCR_HEIGHT/2 + 2);
    draw_text_center("PRESS ESC TO EXIT GAME", 22, SCR_HEIGHT/2 + 5, CINZA, BRANCO);
    draw_text_center("PRESS 'R' TO RESTART", 20, SCR_HEIGHT/2 + 7, CINZA, BRANCO);
    printscr();
}


void update_game(){
    int y, colision;

    bird_update(flappy);
    pipes_update();

    colision = pipes_check_colision(flappy);

    if(colision == -1) bird_kill(flappy);
    else if(colision == 1 && bird_is_alive(flappy))
        score ++;

    if(score >= MAX_SCORE){
        keyboard_set_handler(over_key_handler);
        current = GAME_WIN;
    }
    
    y = bird_get_pos(flappy);

    if(y > SCR_HEIGHT - 5){
        bird_kill(flappy);
        bird_set_pos(flappy, SCR_HEIGHT - 4);
        current = GAME_OVER;
        keyboard_set_handler(over_key_handler);
    }
}

void draw_game(){
    draw_background();
    pipes_draw();
    draw_score(SCR_WIDTH/2 - 3, 1);
    bird_draw(flappy);
    printscr();
}

void update_menu(){
    int y;

    bird_update(flappy);
    y = bird_get_pos(flappy);

    if(y > SCR_HEIGHT/2 - 1)
        bird_flap(flappy);
}

void draw_menu(){
    draw_background();
    draw_menu_text();
    bird_draw(flappy);
    printscr();
}

void game_run(){
    while (current != GAME_EXIT){
        usleep(16666 * 7);

        switch (current){
        case GAME_INIT:
            current = GAME_MENU;
            keyboard_set_handler(menu_key_handler);
            break;
        case GAME_MENU:
            update_menu();
            draw_menu();
            break;
        case GAME_RUNNING:
            update_game();
            draw_game();
            break;
        case GAME_PAUSED:
            break;
        case GAME_OVER:
            if (score > high_score)
                high_score = score;            
            sleep(1);
            draw_over();
            break;
        case GAME_WIN:
            if (score > high_score)
                high_score = score;      
            sleep(1);
            draw_win();
        case GAME_EXIT:
            break;
        default:
            printf("An unexpected error occurred\n");
            current = GAME_EXIT;
            sleep(1);
            break;
        }
    }


    keyboard_shutdown();
    screen_delete();
    bird_destroy(&flappy);
    pipes_destroy();

    FILE * f = fopen("high_score.bin", "wb");
    if (f != NULL){
        fwrite(&high_score, sizeof(int), 1, f);
        fclose(f);
    } else printf("Error: High Score saving Failed\n");
}

void menu_key_handler(char c){
    switch (c){
    case KEY_ESCAPE:
        current = GAME_EXIT;
        break;
    default:
        score = 0;
        current = GAME_RUNNING;
        keyboard_set_handler(game_key_handler);
        break;
    }
}

void game_key_handler(char c){
    switch (c){
    case KEY_ESCAPE:
        keyboard_set_handler(paused_key_handler);
        current = GAME_PAUSED;
        draw_pause_text();
        break;
    case KEY_SPACE:
        bird_flap(flappy);
        break;
    }
}

void paused_key_handler(char c){
    switch (c){
    case KEY_ESCAPE:
        current = GAME_EXIT;
        break;
    case KEY_SPACE:
        current = GAME_RUNNING;
        keyboard_set_handler(game_key_handler);
        break;
    case KEY_R:
    case KEY_r:
        game_restart();
        break;
    }
}

void over_key_handler(char c){
    switch (c){
    case KEY_ESCAPE:
        current = GAME_EXIT;
        break;
    case KEY_R:
    case KEY_r:
        game_restart();
        break;
    }
}