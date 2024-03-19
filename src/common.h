#ifndef EPHO_EDITOR_COMMON_H
#define EPHO_EDITOR_COMMON_H

#include <stdint.h>
#include "v2.h"

uint8_t boundary_overflow(V2 pt, int min_x, int max_x, int min_y, int max_y);

#endif // EPHO_EDITOR_COMMON_H
