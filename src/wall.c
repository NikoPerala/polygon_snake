#include "wall.h"
#include "polygon.h"

void
wall_init(Wall *wall, V2 pt1, V2 pt2, int thickness)
{
    wall->baseline[0] = pt1;
    wall->baseline[1] = pt2;
    wall->thickness = thickness;
    wall_calculate_points(wall);
}

void 
wall_update_point(Wall *wall, V2 point, int id)
{
    wall->baseline[id] = point;
    wall_calculate_points(wall);
}

void
wall_calculate_points(Wall *wall)
{
    float dx = wall->baseline[1].x - wall->baseline[0].x;
    float dy = wall->baseline[1].y - wall->baseline[0].y;
    wall->angle = atan2(dy, dx);

    float top_angle = wall->angle - HALFPI;
    float bottom_angle = wall->angle + HALFPI;

    float pt1_top_x = wall->baseline[0].x + wall->thickness * cos(top_angle);
    float pt1_top_y = wall->baseline[0].y + wall->thickness * sin(top_angle);

    float pt1_bottom_x = wall->baseline[0].x + wall->thickness * cos(bottom_angle);
    float pt1_bottom_y = wall->baseline[0].y + wall->thickness * sin(bottom_angle);

    float pt2_top_x = wall->baseline[1].x + wall->thickness * cos(top_angle);
    float pt2_top_y = wall->baseline[1].y + wall->thickness * sin(top_angle);
    float pt2_bottom_x = wall->baseline[1].x + wall->thickness * cos(bottom_angle);
    float pt2_bottom_y = wall->baseline[1].y + wall->thickness * sin(bottom_angle);

    wall->points[0] = (V2) { pt1_top_x, pt1_top_y }; 
    wall->points[1] = (V2) { pt2_top_x, pt2_top_y }; 
    wall->points[2] = (V2) { pt2_bottom_x, pt2_bottom_y }; 
    wall->points[3] = (V2) { pt1_bottom_x, pt1_bottom_y }; 
}

void
wall_display(eCanvas *canvas, Wall wall)
{
    eFillPolygon(canvas, wall.points, WALL_POINT_AMOUNT, 0xff00a000);
}

