#include "snakegame.h"
#include "v2.h"
#include "polygon.h"

#include <math.h>
#include <stdio.h>

#define HEADPOINTS_AMOUNT 5
#define SNAKE_THICKNESS 7

void init_snake(Snake *snake)
{
    snake->length = SNAKE_START_LENGTH;
    snake->direction = SNAKE_START_ANGLE;
    snake->speed = SNAKE_START_SPEED;
    snake->capacity = SNAKE_POSITION_AMOUNT;

    snake->members = malloc(sizeof(SnakeMember) * snake->capacity);
    for (int i = 0; i < SNAKE_START_LENGTH; ++i){
        snake->members[i] = (SnakeMember) { 0 };
        snake->members[i].position = SNAKE_START_POINT;
        snake->members[i].wing1 = SNAKE_START_POINT;
        snake->members[i].wing2 = SNAKE_START_POINT;
    }
}

void snake_reset(Snake *snake)
{
    snake->length = SNAKE_START_LENGTH;
    snake->direction = SNAKE_START_ANGLE;
    snake->speed = SNAKE_START_SPEED;
    
    for (int i = 0; i < SNAKE_START_LENGTH; ++i){
        snake->members[i].teleportation = 0;
        snake->members[i].position = SNAKE_START_POINT;
        snake->members[i].wing1 = SNAKE_START_POINT;
        snake->members[i].wing2 = SNAKE_START_POINT;
    }
}

void free_snake(Snake *snake)
{
    free(snake->members);
}

BOUNDARY_OVERFLOW check_boundarys(V2 *pos)
{
    int retval = OVERFLOW_NONE;

    if (pos->x >= W_WIDTH) {
        pos->x -= W_WIDTH;
        retval = OVERFLOW_RIGHT;
    }
    if (pos->x < 0){ 
        pos->x += W_WIDTH;
        retval = OVERFLOW_LEFT;
    }
    if (pos->y >= W_HEIGHT){ 
        pos->y -= W_HEIGHT;
        retval = OVERFLOW_BOTTOM;
    }
    if (pos->y < 0) {
        pos->y += W_HEIGHT;
        retval = OVERFLOW_TOP;
    }
    return retval;
}

int snake_move(Snake *snake)
{
    // Shift positions
    for (int i = snake->length - 1; i > 0; --i){
        snake->members[i] = snake->members[i - 1];
        V2 points[] = {snake->members[i].wing1, 
                       snake->members[i].wing2, 
                       snake->members[i - 1].wing2, 
                       snake->members[i - 1].wing1};
        if (i > SNAKE_START_LENGTH && 
            check_collision_polygon(points, 4, snake->headpoints, 4)) return 0;
    }
    // End of

    V2 new_pos = V2_from_polar(snake->members[0].position, snake->speed, snake->direction);

    SnakeMember *memberPtr;

    memberPtr = &snake->members[0];

    memberPtr->teleportation = check_boundarys(&new_pos);
    if (memberPtr->teleportation) snake->members[1].teleportation = memberPtr->teleportation;
    
    memberPtr->position = new_pos; 

    float angle1 = snake->direction - HALFPI;
    float angle2 = angle1 + PI;

    int thickness = SNAKE_THICKNESS;

    memberPtr->wing1 = V2_from_polar(new_pos, thickness, angle1);
    memberPtr->wing2 = V2_from_polar(new_pos, thickness, angle2);

    float step = PI / (HEAD_TAIL_PT_AMOUNT - 1);

    // Headpoints
    snake->headpoints[0] = snake->members[0].wing1;
    for (int i = 1; i < HEAD_TAIL_PT_AMOUNT - 1; ++i){
        snake->headpoints[i] = V2_from_polar(new_pos, thickness, angle1 + i * step);
    }
    snake->headpoints[4] = snake->members[0].wing2;

    // Tailpoints
    memberPtr = &snake->members[snake->length - 1];
    float dx = memberPtr->wing2.x - memberPtr->wing1.x;
    float dy = memberPtr->wing2.y - memberPtr->wing1.y;
    float tail_dir = atan2(dy, dx);
    snake->tailpoints[0] = memberPtr->wing2;
    for (int i = 1; i < HEAD_TAIL_PT_AMOUNT - 1; ++i){
        snake->tailpoints[i] = V2_from_polar(memberPtr->position, thickness, tail_dir + i * step);
    }
    snake->tailpoints[4] = memberPtr->wing1;

    return 1;
}

void snake_display(Snake *snake)
{

    // These to config / constants
    uint32_t c = 0x0080a0ff;
    int thickness = 5;
    float step = 360.f / snake->length;

    for (int i = snake->length - 1; i >= 0; --i){
        c = snake->members[i].teleportation > 0 ? 0xff0000ff : 0x0080a0ff;
        if (i > 0 && i <= snake->length - 1 && snake->members[i].teleportation == OVERFLOW_NONE){ 
            V2 pts[] = { 
                snake->members[i].wing1, snake->members[i].wing2,
                snake->members[i - 1].wing2, snake->members[i - 1].wing1
            };
            fill_polygon(pts, 4, c);
        }
        if (i == 0) fill_polygon(snake->headpoints, HEAD_TAIL_PT_AMOUNT, c);
        if (i == snake->length - 1) fill_polygon(snake->tailpoints, HEAD_TAIL_PT_AMOUNT, c);
    }
}

uint8_t snake_eat(Snake *snake)
{
    int tmp = snake->length;
    snake->speed += 0.1;
    snake->length += SNAKE_FOOD_LENGTH;

    if (snake->length >= snake->capacity){
        snake->capacity *= 2;
        snake->members = realloc(snake->members, sizeof(SnakeMember) * snake->capacity);
        if (snake->members == NULL) return 1;
    }
    for (tmp; tmp <= snake->length; ++tmp){
        snake->members[tmp] = snake->members[tmp - 1];
    }

    return 0;
}
