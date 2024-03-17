#ifndef EPHO_SNAKEGAME_H
#define EPHO_SNAKEGAME_H

#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define W_WIDTH 1000
#define W_HEIGHT 1000
#define W_TITLE "Snake game"

#define SNAKE_START_LENGTH 30
#define SNAKE_START_SPEED 4.f
#define SNAKE_START_ANGLE 3
#define SNAKE_POSITION_AMOUNT 500
#define SNAKE_ROTATION_SPEED 0.15
#define SNAKE_RADIUS 3
#define SNAKE_FOOD_LENGTH 10

#define WALL_THICKNESS 2

#define ITEM_RADIUS 15
#include "constants.h"
#include "v2.h"
#include "drawfuncs.h"
#include "item.h"
#include "wall.h"
#include "polygon.h"
#include "snake.h"


typedef enum {
    GAME_OVER,
    GAME_ON
} GAME_STATES;

typedef struct Line {
    V2 p1;
    V2 p2;
} Line;

// Let's start from small
typedef struct Level {
    uint16_t screen_width; //quick && dirty
    uint16_t screen_height; //quick && dirty

    V2 startpoint;
    float startangle;

    uint8_t loading_status;
    uint8_t wall_amount;
    uint8_t wall_count;
    Wall *walls;
} Level;

Level get_level();
void free_level(Level *level);
uint8_t process_line(char *line, Level *level);
uint8_t create_level_from_file(const char *path, Level *level);

#endif // EPHO_SNAKEGAME_H
