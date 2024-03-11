#ifndef EPHO_ITEM_H
#define EPHO_ITEM_H

#include <stdint.h>
#include "v2.h"
#include "drawfuncs.h"

typedef enum {
    ITEM_NONE,
    ITEM_APPLE,
    ITEM_COUNT
} ITEM_TYPES;

typedef struct Item {
    int type;
    int point_amount;
    V2 *points;
    V2 position;
    float angle;
} Item;

uint8_t initialize_item(Item *item);
void free_item(Item *item);
void display_item(Item *item);
void display_apple(Item *item);


#endif // EPHO_ITEM_H
