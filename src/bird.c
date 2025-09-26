#include <stdlib.h>
#include <stdio.h>
#include "screen.h"
#include "bird.h"
#include "config.h"

static char bird_sprite[2][3][4] = 
{
    {// falling frame
        {' ', '(', '@', '>'}, //  (@>
        {'/', '|', '|', ' '}, // \||
        {' ', '\"','\"',' '}  //  ""
    },
    {// flying frame
        {' ', '(', '@', '>'}, //  (@>
        {'\\','|', '|', ' '}, // /||
        {' ', '\"','\"',' '}  //  ""
    }
};

// Bird foreground colors
const Color bird_fore[3][4] =
{
    {CLEAR, AZUL, BRANCO, VERMELHO_CLARO},
    {AZUL, AZUL, AZUL, CLEAR},
    {CLEAR, PRETO, PRETO, CLEAR}
};

// Bird background colors
const Color bird_back[3][4] =
{
    {CLEAR, AMARELO, AMARELO, CLEAR},
    {AMARELO, AMARELO, AMARELO, CLEAR},
    {CLEAR, CLEAR, CLEAR, CLEAR}
};

struct _bird{
    int pos;
    int v;
    bool alive;
};

Bird * bird_create(){
    Bird * b = (Bird *) malloc(sizeof(Bird));
    if(b == NULL) exit(0);

    b->alive = true;
    b->pos = SCR_HEIGHT/2 - 1;
    b->v = 0;

    return b;
}

void bird_destroy(Bird * b){
    if (b == NULL) return;

    free(b);
    b = NULL;
}

int bird_get_pos(Bird * b){
    if (b == NULL) return - 1;

    return b->pos;
}

int bird_get_velocity(Bird * b){
    if (b == NULL) return __INT32_MAX__;

    return b->v;
}

bool bird_is_alive(Bird * b){
    if (b == NULL) return false;

    return b->alive;
}

void bird_update(Bird * b){
    if(b == NULL) return;
    if(!b->alive) return;

    b->pos += b->v;

    if (b->v + GRAVITY < MAX_FALL_SPEED)
        b->v += GRAVITY;
}

void bird_kill(Bird * b){
    if(b == NULL) return;

    b->alive = false;
    bird_sprite[0][0][2] = 'X';
    bird_sprite[1][0][2] = 'X';
}

void bird_respawn(Bird * b){
    if (b == NULL) return;

    b->alive = true;
    b->pos = SCR_HEIGHT/2 - 1;
    b->v = 0;
    bird_sprite[0][0][2] = '@';
    bird_sprite[1][0][2] = '@';
}

void bird_flap(Bird * b){
    if(b == NULL) return;
    if(!b->alive) return;

    b->v = FLAP_VELOCITY;

}

void bird_draw(Bird * b){
    if(b == NULL) return;

    int v = (b->v > 0)? 1 : 0; // if velocity is positive -> falling frame, else -> flying frame

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 4; j++){
            set_pixel(SCR_WIDTH/2 - 4 + j, b->pos + i, bird_sprite[v][i][j], bird_fore[i][j], bird_back[i][j]);
        }
        
    }
    
}