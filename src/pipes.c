#include <stdlib.h>
#include <stdio.h>
#include "screen.h"
#include "config.h"
#include "pipes.h"

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
    new_p->pos_x = SCR_WIDTH-3;
    new_p->pos_y = SCR_HEIGHT/2;

    if(head == NULL){
        head = new_p;
        return;
    }

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
        else
            p = &(*p)->next;
    }
    
}

int pipes_check_colision(Bird * b){
    if(b == NULL) return 0;
    
    Pipe * aux = head;

    while (aux != NULL){
        
    }
    return 0;
}

void pipes_draw(){
    Pipe * aux = head;

    while(aux != NULL){
        draw_rect(aux->pos_x, -1, 3, aux->pos_y - PIPE_GAP/2, ' ', PRETO, VERDE, true);
        draw_rect(aux->pos_x, aux->pos_y + PIPE_GAP/2, 3, SCR_HEIGHT - aux->pos_y + PIPE_GAP/2, ' ', PRETO, VERDE, true);
        aux = aux->next;
    }
}