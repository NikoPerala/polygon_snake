#ifndef EPHO_V2_H
#define EPHO_V2_H

#include <stdlib.h>

typedef struct V2 
{
    float x;
    float y;
} V2;

V2 V2_random(int minx, int maxx, int miny, int maxy);
float V2_sqr_distance(V2 a, V2 b);
float V2_distance(V2 a, V2 b);
float V2_dot_product(V2 a, V2 b);
V2 V2_from_polar(V2 pt, int r, float angle);

#endif // EPHO_V2_H

