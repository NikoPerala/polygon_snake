#include "pointsystem.h"
#include "snakegame.h"

uint8_t initialize_pointsystem(PointSystem *ps, PointSystemType type)
{
    ps->type = type;
    switch (ps->type){
        case WALL:
            ps->point_amount = 6;
            ps->movable_points = 2;
            break;
        case ARROW:
            ps->point_amount = 4;
            ps->movable_points = 1;
            break;
    }
    ps->angle = 0;
    ps->points = (V2*) malloc(sizeof(V2) * ps->point_amount);
    if (ps->points == NULL) return 1;
    for (int i = 0; i < ps->point_amount; ++i){
        ps->points[i] = (V2) { 0 };
    }
    
    return 0;
}

void display_pointsystem(eCanvas *canvas, PointSystem *ps)
{
    switch (ps->type){
        case WALL:
            display_wall(canvas, ps, 0xff00A000);
            break;
        case ARROW:
            display_arrow(canvas, ps, 0xFFA0A0A0);
            break;
    }
}
void update_pointsystem(PointSystem *ps)
{
    switch (ps->type){
        case WALL:
            update_wall(ps);
            break;
        case ARROW:
            update_arrow(ps->points, ps->angle);
            break;
    }
}

void set_pointsystem_point(PointSystem *ps, int id, V2 pt)
{
    ps->points[id] = pt;
}

#define ARROW_LENGTH 30
#define ARROW_POINTER_LENGTH 10
#define ARROW_ANGLE QUARTERPI

void display_wall(eCanvas *canvas, PointSystem *ps, uint32_t color)
{
    V2 points[4] = {
        ps->points[2],
        ps->points[3],
        ps->points[4],
        ps->points[5],
    };

    eFillPolygon(canvas, points, 4, color);
}

void update_wall(PointSystem *ps)
{
    float dx = ps->points[1].x - ps->points[0].x;
    float dy = ps->points[1].y - ps->points[0].y;

    ps->angle = atan2(dy, dx) + HALFPI;

    ps->points[2].x = ps->points[0].x + WALL_THICKNESS * cos(ps->angle);
    ps->points[2].y = ps->points[0].y + WALL_THICKNESS * sin(ps->angle);

    ps->points[3].x = ps->points[1].x + WALL_THICKNESS * cos(ps->angle);
    ps->points[3].y = ps->points[1].y + WALL_THICKNESS * sin(ps->angle);

    ps->points[4].x = ps->points[1].x + WALL_THICKNESS * cos(ps->angle + PI);
    ps->points[4].y = ps->points[1].y + WALL_THICKNESS * sin(ps->angle + PI);

    ps->points[5].x = ps->points[0].x + WALL_THICKNESS * cos(ps->angle + PI);
    ps->points[5].y = ps->points[0].y + WALL_THICKNESS * sin(ps->angle + PI);
}

void update_arrow(V2 *points, float angle)
{ 
    points[1] = (V2){
            points[0].x + ARROW_LENGTH * cos(angle),
            points[0].y + ARROW_LENGTH * sin(angle)
                    };
    points[2] = (V2) {
            points[1].x + ARROW_POINTER_LENGTH * cos(angle - THREEQUARTERPI),
            points[1].y + ARROW_POINTER_LENGTH * sin(angle - THREEQUARTERPI)
                           };
    points[3] = (V2) {
            points[1].x + ARROW_POINTER_LENGTH * cos(angle + THREEQUARTERPI),
            points[1].y + ARROW_POINTER_LENGTH * sin(angle + THREEQUARTERPI)
                           };
}

void display_arrow(eCanvas *canvas, PointSystem *ps, uint32_t color)
{
    eDrawLine(canvas, 
              ps->points[0].x, ps->points[0].y,
              ps->points[1].x, ps->points[1].y, color);
    eDrawLine(canvas, 
              ps->points[1].x, ps->points[1].y,
              ps->points[2].x, ps->points[2].y, color);
    eDrawLine(canvas, 
              ps->points[1].x, ps->points[1].y,
              ps->points[3].x, ps->points[3].y, color);
}

