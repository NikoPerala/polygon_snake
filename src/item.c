#include <math.h>
#include <stdlib.h>

#include "item.h"
#include "snakegame.h"

#define APPLE_RADIUS 15
#define APPLE_POINTS 6

void free_item(Item *item)
{
    free(item->points);
}

uint8_t initialize_item(Item *item)
{

    switch (item->type){
        case ITEM_APPLE:
            item->point_amount = APPLE_POINTS;
            break;
    }

    item->points = malloc(sizeof(V2) * item->point_amount);
    if (item->points == NULL) return 1;

    for (int i = 0; i < item->point_amount; ++i){
        item->points[i] = (V2) { 0 };
    }

    return 0;
}

void display_item(eCanvas *canvas, Item *item)
{
    switch (item->type){
        case ITEM_APPLE:
            display_apple(canvas, item);
    }
}

void display_apple(eCanvas *canvas, Item *item)
{
    float step = TAU / item->point_amount;
    
    for (int i = 0; i < item->point_amount; ++i){
         float x = item->position.x + APPLE_RADIUS * cos(item->angle + step * i);
         float y = item->position.y + APPLE_RADIUS * sin(item->angle + step * i);
         item->points[i] = (V2) { x, y }; 
    }

    eFillPolygon(canvas, item->points, item->point_amount, 0xff0000ff);
    
}
