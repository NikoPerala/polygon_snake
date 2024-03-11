#include "drawfuncs.h"
#include <math.h>

#define EPSILON 1e-6

int signage_is_same(float a, float b)
{
    return ((a < 0 && b < 0) || (a > 0 && b > 0));
}
int line_line_intersection(V2 lp1, V2 lp2, V2 lp3, V2 lp4, V2 *pt)
{
    float x1 = lp1.x;
    float y1 = lp1.y;
    float x2 = lp2.x;
    float y2 = lp2.y;
    float x3 = lp3.x;
    float y3 = lp3.y;
    float x4 = lp4.x;
    float y4 = lp4.y;

    float num_t =  ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4));
    float den   =  ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

    // Check if lines are parallel (den == 0)
    if (fabs(den) < EPSILON) return 0;

    float t = num_t / den;

    // Check if the intersection point is within the line segments
    if (t < 0 || t > 1) return 0;

    // Calculate the intersection point
    pt->x = x1 + t * (x2 - x1);
    pt->y = y1 + t * (y2 - y1);

    return 1;
}

void fill_polygon(V2 *points, int point_amount, uint32_t color) {
    if (point_amount < 3) {
        // Cannot fill a polygon with less than 3 points
        return;
    }

    // Find the minimum and maximum y-coordinates to determine the scanlines
    int min_y = points[0].y;
    int max_y = points[0].y;
    for (int i = 1; i < point_amount; i++) {
        if (points[i].y < min_y) {
            min_y = points[i].y;
        }
        if (points[i].y > max_y) {
            max_y = points[i].y;
        }
    }

    // Scanline algorithm
    for (int y = min_y; y <= max_y; y++) {
        // Store the intersection points
        int intersections[point_amount];
        int num_intersections = 0;

        // Find the intersections with polygon edges
        for (int i = 0; i < point_amount; i++) {
            int j = (i + 1) % point_amount; // Next point
            if ((points[i].y < y && points[j].y >= y) || (points[j].y < y && points[i].y >= y)) {
                // If the edge crosses the scanline
                // Find the intersection point
                intersections[num_intersections++] = points[i].x + (double)(y - points[i].y) / (points[j].y - points[i].y) * (points[j].x - points[i].x);
            }
        }

        // Sort the intersections
        for (int i = 0; i < num_intersections - 1; i++) {
            for (int j = 0; j < num_intersections - i - 1; j++) {
                if (intersections[j] > intersections[j + 1]) {
                    int temp = intersections[j];
                    intersections[j] = intersections[j + 1];
                    intersections[j + 1] = temp;
                }
            }
        }

        // Draw lines between the intersections to fill the polygon
        for (int i = 0; i < num_intersections; i += 2) {
            draw_line((V2){intersections[i], y}, (V2){intersections[i + 1], y}, color);
        }
    }
}

int overlap_on_axis(V2 *points1, int points1_amount,
                    V2 *points2, int points2_amount,
                    V2 axis)
{
    float min1 = V2_dot_product(axis, points1[0]);
    float max1 = min1;
    float min2 = V2_dot_product(axis, points2[0]);
    float max2 = min2;

    for (int i = 0; i < points1_amount; ++i){
        float projection = V2_dot_product(axis, points1[i]);
        if (projection < min1) min1 = projection;
        if (projection > max1) max1 = projection;
    }

    for (int i = 0; i < points2_amount; ++i){
        float projection = V2_dot_product(axis, points2[i]);
        if (projection < min2) min2 = projection;
        if (projection > max2) max2 = projection;
    }

    float overlap1 = max1 - min2;
    float overlap2 = max2 - min1;
    float overlap = (overlap1 < overlap2) ? overlap1 : overlap2;

    if (overlap < 0) return 0;

    return 1;
}

int check_collision_polygon(V2 *points1, int points1_amount,
                            V2 *points2, int points2_amount)
{
    const int axes_amount = points1_amount + points2_amount;

    V2 axes[axes_amount];
    for (int i = 0; i < axes_amount; ++i){
        float dx, dy;
        if (i < points1_amount - 1) {
            dx = points1[i + 1].x - points1[i].x;
            dy = points1[i + 1].y - points1[i].y;
        } else if (i == points1_amount - 1){
            dx = points1[0].x - points1[i].x;
            dy = points1[0].y - points1[i].y;
        } else if (i >= points1_amount && i < axes_amount - 1){
            int id = i - points1_amount;
            dx = points2[id + 1].x - points2[id].x;
            dy = points2[id + 1].y - points2[id].y;
        } else if (i == axes_amount - 1){
            dx = points2[0].x - points2[i].x;
            dy = points2[0].y - points2[i].y;
        }
        axes[i] = (V2) { dx, dy };
    }

    for (int i = 0; i < axes_amount; ++i){
        V2 axis = { -axes[i].y, axes[i].x };

        if (!overlap_on_axis(points1, points1_amount, points2, points2_amount, axis)) return 0;
    }

    return 1;
}

