#ifndef EPHO_POINTSYSTEM_H
#define EPHO_POINTSYSTEM_H

#include <stdint.h>
#include "v2.h"
#include "egfx.h"
#include "constants.h"

typedef enum {                
    NONE, 
    WALL,
    ARROW,
    SNAKE_SEGMENT,
    SNAKE_END
} PointSystemType;

typedef struct PointSystem {
    PointSystemType type;      
    float angle;               
    int point_amount;          
    int movable_points;        
    V2 *points;
    V2 offset;
} PointSystem; 

uint8_t initialize_pointsystem(PointSystem *ps, PointSystemType type);
void display_pointsystem(eCanvas *canvas, PointSystem *ps, uint32_t color);
void update_pointsystem(PointSystem *ps);

void update_arrow(V2 *points, float angle);
void display_arrow(eCanvas *canvas, PointSystem *ps, uint32_t color);

void update_wall(PointSystem *ps);
void display_wall(eCanvas *canvas, PointSystem *ps, uint32_t color);
void move_wall(PointSystem *ps, V2 mouse, int *state_ready);

#endif // EPHO_POINTSYSTEM_H

