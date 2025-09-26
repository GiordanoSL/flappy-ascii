#ifndef BIRD_H
#define BIRD_H

typedef struct _bird Bird;

Bird * bird_create();


void bird_destroy(Bird ** b);

int bird_get_pos(Bird * b);

int bird_get_velocity(Bird * b);

bool bird_is_alive(Bird * b);

void bird_update(Bird * b);

void bird_kill(Bird * b);

void bird_respawn(Bird * b);

void bird_flap(Bird * b);

void bird_draw(Bird * b);


#endif // BIRD_H