#ifndef EPHO_SNAKE_H
#define EPHO_SNAKE_H

#define HEAD_TAIL_PT_AMOUNT 5
#include "v2.h"
#include "egfx.h"

typedef enum {
    OVERFLOW_NONE   = 0,
    OVERFLOW_LEFT   = 1,
    OVERFLOW_RIGHT  = 2,
    OVERFLOW_TOP    = 3,
    OVERFLOW_BOTTOM = 4
} BOUNDARY_OVERFLOW;

typedef struct SnakeMember {
    V2 position;
    V2 wing1;
    V2 wing2;
    int teleportation;
} SnakeMember;

typedef struct Snake {
    SnakeMember *members;
    V2 headpoints[HEAD_TAIL_PT_AMOUNT];
    V2 tailpoints[HEAD_TAIL_PT_AMOUNT];
    float direction;
    int length;
    int capacity;
    float speed;
} Snake;

void init_snake(Snake *snake);
void free_snake(Snake *snake);

void snake_shift_members(Snake *snake);
int snake_move(Snake *snake);
void snake_display(eCanvas *canvas, Snake *snake);
uint8_t snake_eat(Snake *snake);
void snake_reset(Snake *snake, V2 startpoint, float angle);

#endif // EPHO_SNAKE_H
