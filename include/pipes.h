#ifndef PIPES_H
#define PIPES_H

#include "bird.h"

typedef struct _pipe Pipe;

void pipe_create();
void pipes_destroy();
void pipes_update();
int pipes_check_colision(Bird * b);
void pipes_draw();

#endif // PIPES_H