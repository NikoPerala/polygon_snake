#include "drawfuncs.h"

Vector2 V2_to_Vector2(V2 in)
{
    return (Vector2) { in.x, in.y };
}

Color uint32_to_Color(uint32_t color)
{
    Color c;

    c.r = (color >> (3 * 8)) & 0xff;
    c.g = (color >> (2 * 8)) & 0xff;
    c.b = (color >> (1 * 8)) & 0xff;
    c.a = (color >> (0 * 8)) & 0xff;

    return c;
}

void draw_line(V2 pt1, V2 pt2, uint32_t color)
{
    Color c = uint32_to_Color(color);
    Vector2 p1 = V2_to_Vector2(pt1);
    Vector2 p2 = V2_to_Vector2(pt2);
    DrawLineV(p1, p2, c);
}

void draw_circle(V2 pt, float radius, uint32_t color)
{
    Color c = uint32_to_Color(color);
    Vector2 cp  = V2_to_Vector2(pt); 

    DrawCircleV(cp, radius, c);
}

void draw_polygon_lines(V2 *points, int point_amount, uint32_t color)
{
    for (int i = 0; i < point_amount; ++i){
        if (i == point_amount - 1) draw_line(points[i], points[0], color);
        else draw_line(points[i], points[i + 1], color);
    }
}
