#ifndef EPHO_EDITOR_H
#define EPHO_EDITOR_H

#include "egfx.h"

typedef enum {
    WALL,
    ARROW,
} PointSystemType;

typedef struct PointSystem {
    PointSystemType type;
    float angle;
    int point_amount;
    int movable_points;
    V2 *points;
} PointSystem;

typedef enum {
    EDITOR_STATE_NONE,
    EDITOR_STATE_MOVE_POINT
} EditorState;

typedef struct Editor {
    EditorState state;
    PointSystem *pointsystems;
    unsigned int pointsystem_capacity;
    unsigned int pointsystem_count;
    unsigned int pointsystem_id;
    unsigned int point_id;
    int hovered;
} Editor;

uint8_t init_editor(Editor *ed);
void free_editor(Editor *ed); 
int double_pointsystem_capacity(Editor *ed);
int write_level(Editor *ed, char *name);

int is_points_near(V2 pt1, V2 pt2, int tolerance);

void draw_pt_crosshair(eCanvas *canvas, V2 pt);

uint8_t initialize_pointsystem(PointSystem *ps, PointSystemType type);
void display_pointsystem(eCanvas *canvas, PointSystem *ps);
void update_pointsystem(PointSystem *ps);
uint8_t add_pointsystem(Editor *ed, PointSystemType type);

void update_arrow(V2 *points, float angle);
void display_arrow(eCanvas *canvas, PointSystem *ps, uint32_t color);

void update_wall(PointSystem *ps);
void display_wall(eCanvas *canvas, PointSystem *ps, uint32_t color);

#endif // EPHO_EDITOR_H
