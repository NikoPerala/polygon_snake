#include "common.h"

uint8_t boundary_overflow(V2 pt, int min_x, int max_x, int min_y, int max_y)
{
    return ( pt.x < min_x || pt.x >= max_x || pt.y < min_y || pt.y >= max_y );
}
