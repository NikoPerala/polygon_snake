#ifndef EPHO_EDITOR_H
#define EPHO_EDITOR_H

#include "egfx.h"
#include "pointsystem.h"

#define EDITOR_STATE_LIST       \
    X(EDITOR_STATE_NONE)        \
    X(EDITOR_STATE_MOVE_POINT)  \
    X(EDITOR_STATE_MOVE_WALL)   \
    X(EDITOR_STATE_COUNT) 

#define X(name) name,
typedef enum EDITOR_STATE_ENUM {
    EDITOR_STATE_LIST
} EditorState;
#undef X

#define X(name) #name,
const char *EditorStateNames[] = {
    EDITOR_STATE_LIST
};

typedef struct Editor {
    EditorState state;
    PointSystem *pointsystems;
    unsigned int pointsystem_capacity;
    unsigned int pointsystem_count;
    int pointsystem_id;
    int point_id;
    float angle;
    int hovered;

    V2 offset;
    int state_ready;
} Editor;

uint8_t init_editor(Editor *ed);
void free_editor(Editor *ed); 
int double_pointsystem_capacity(Editor *ed);
int write_level(Editor *ed, char *name);

int is_points_near(V2 pt1, V2 pt2, int tolerance);
float distance_from_line(V2 pt, V2 pt1, V2 pt2);

void draw_pt_crosshair(eCanvas *canvas, V2 pt);

uint8_t add_pointsystem(Editor *ed, PointSystemType type);

#endif // EPHO_EDITOR_H
