#include <stdlib.h>
#include <stdio.h>
#include "screen.h"
#include "config.h"
#include "pipes.h"
#include <unistd.h>

Color pipe_fore = PRETO;
Color pipe_back = VERDE;

struct _pipe
{
    int pos_y;
    int pos_x;
    Pipe * next;
};

static Pipe * head = NULL;

void pipe_create(){
    Pipe * new_p = (Pipe *) malloc(sizeof(Pipe));
    if(new_p == NULL){
        perror("Allocation failed for new Pipe");
        exit(1);
    }

    new_p->next = NULL;
    new_p->pos_x = SCR_WIDTH;

    if(head == NULL){
        new_p->pos_y = SCR_HEIGHT/2;
        head = new_p;
        return;
    }

    new_p->pos_y = rand() % (SCR_HEIGHT - PIPE_GAP - 5) + 2;

    Pipe * aux = head;
    while(aux->next != NULL){
        aux = aux->next;
    }

    aux->next = new_p;
}

void pipe_delete(Pipe ** p){
    if (*p == NULL) return;
    
    Pipe * next = (*p)->next;

    free(*p);
    *p = next;
}

void pipes_update(){
    Pipe ** p = &head;

    while (*p != NULL){
        (*p)->pos_x -= PIPE_SPEED;
        if((*p)->pos_x < -2)
            pipe_delete(p);
        else{
            if((*p)->pos_x == SCR_WIDTH - PIPE_SPAWN_INTERVAL - 1)
                pipe_create();
            p = &(*p)->next;
        }
    }
    
}

int pipes_check_colision(Bird * b){
    if(b == NULL) return 0;
    
    Pipe * aux = head;
    int b_pos = bird_get_pos(b);


    while (aux != NULL){
        if(aux->pos_x + PIPE_WIDTH - 1 >= BIRD_X_POS && aux->pos_x <= BIRD_X_POS + BIRD_WIDTH - 1){
            if(b_pos < aux->pos_y || b_pos + BIRD_HEIGHT - 1 > aux->pos_y + PIPE_GAP)                
                return 1;
            else return 0;
        }
        aux = aux->next;
    }
    return 0;
}

void pipes_draw(){
    Pipe * aux = head;

    while(aux != NULL){
        draw_rect(aux->pos_x, -1, PIPE_WIDTH, aux->pos_y + 1, ' ', PRETO, VERDE, true);
        draw_rect(aux->pos_x, aux->pos_y + PIPE_GAP, PIPE_WIDTH, SCR_HEIGHT - aux->pos_y + PIPE_GAP, ' ', pipe_fore, pipe_back, true);
        aux = aux->next;
    }
}