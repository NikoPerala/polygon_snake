#ifndef EPHO_DRAWFUNCS_H
#define EPHO_DRAWFUNCS_H

#include <raylib.h>
#include <stdint.h>
#include "v2.h"

void draw_line(V2 pt1, V2 pt2, uint32_t color);
void draw_circle(V2 pt, float radius, uint32_t color);
void draw_polygon_lines(V2 *points, int point_amount, uint32_t color);

#endif // EPHO_DRAWFUNCS_H
