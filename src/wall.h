#ifndef EPHO_WALL_H
#define EPHO_WALL_H

#include <math.h>
#include <stdint.h>
#include "v2.h"
#include "constants.h"

#define WALL_POINT_AMOUNT 4

typedef struct Wall {
    uint8_t type;
    V2 baseline[2];
//    Line lines[4];
    V2 points[WALL_POINT_AMOUNT];    
    int thickness;
    float angle;
} Wall;

void wall_init(Wall *wall, V2 pt1, V2 pt2, int thickness);
void wall_update_point(Wall *wall, V2 point, int id);
void wall_calculate_points(Wall *wall);
void wall_display(Wall wall);

#endif // EPHO_WALL_H
