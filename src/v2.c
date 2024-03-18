#include <math.h>
#include "v2.h"

float 
V2_sqr_distance(V2 a, V2 b)
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;

    return ( dx * dx + dy * dy );
}

float V2_distance(V2 a, V2 b)
{
    return sqrt(V2_sqr_distance(a, b));
}

float V2_dot_product(V2 a, V2 b)
{
    return (a.x * b.x + a.y * b.y);
}

V2 V2_sub(V2 a, V2 b)
{
    V2 ret = {
        a.x - b.x,
        a.y - b.y
    };

    return ret;
}

V2 V2_add(V2 a, V2 b)
{
    V2 ret = {
        a.x + b.x,
        a.y + b.y
    };

    return ret;
}

V2 V2_delta_vector(V2 pt1, V2 pt2)
{
    V2 delta;

    delta.x = pt2.x - pt1.x;
    delta.y = pt2.y - pt1.y;

    return delta;
}

V2 V2_random(int minx, int maxx, int miny, int maxy)
{
    V2 ret = { (rand() % maxx) + minx, (rand() % maxy) + miny };

    return ret;
}

V2 V2_from_polar(V2 pt, int r, float angle)
{
    V2 ret;

    ret.x = pt.x + r * cos(angle);
    ret.y = pt.y + r * sin(angle);

    return ret;
}
