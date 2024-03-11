#ifndef EPHO_POLYGON_H
#define EPHO_POLYGON_H

#include "v2.h"

typedef struct poly {
    int pt_amount;
    V2 position;
    float radius;
    float angle;
    V2 *points;
} Polygon;

int signage_is_same(float a, float b);
int line_line_intersection(V2 lp1, V2 lp2, V2 lp3, V2 lp4, V2 *pt);
void fill_polygon(V2 *points, int point_amount, uint32_t color);
void draw_polygon(V2 *points, int point_amount, uint32_t color);

int initialize_polygon(Polygon *pol, Vector2 cp, int pt_amount, float radius, float angle);
void free_polygon(Polygon *pol);

int overlap_on_axis(V2 *points1, int points1_amount,
                    V2 *points2, int points2_amount,
                    V2 axis);
int check_collision_polygon(V2 *points1, int points1_amount,
                            V2 *points2, int points2_amount);


#endif // EPHO_POLYGON_H
