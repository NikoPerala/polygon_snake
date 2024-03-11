#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <raylib.h>
#include "snakegame.h"

#define START_WALLS_CAPACITY 10

typedef enum {
    EDITOR_NONE,
    EDITOR_DRAW_WALL,
    EDITOR_EDIT_WALL
} editor_states;

typedef struct Editor {
    int state;
    int current_index;
    int editing_index;
    int wall_count;
    int walls_capacity;
    Wall *walls;
} Editor;

uint8_t init_editor(Editor *ed){
    ed->state = EDITOR_NONE;
    ed->current_index = -1;
    ed->editing_index = -1;
    ed->wall_count = 0;
    ed->walls_capacity = START_WALLS_CAPACITY;
    ed->walls = malloc(sizeof(Wall) * ed->walls_capacity);
    if (ed->walls == NULL) return 1;

    return 0;
}

int write_level(Editor *ed, char *name)
{
    FILE *fp;
    char row[20];

    fp = fopen(name, "w");
    if (fp == NULL) return 1;

    fprintf(fp, "/\n");
    sprintf(row, "W%d\n", ed->wall_count);
    fprintf(fp, row);

    for (int i = 0; i < ed->wall_count; ++i){
        float x1 = ed->walls[i].baseline[0].x / W_WIDTH;
        float y1 = ed->walls[i].baseline[0].y / W_HEIGHT;
        float x2 = ed->walls[i].baseline[1].x / W_WIDTH;
        float y2 = ed->walls[i].baseline[1].y / W_HEIGHT;
        sprintf(row, "w%.3f,%.3f:%.3f,%.3f\n", x1, y1, x2, y2);
        fprintf(fp, row);
    }

    fclose(fp);

    return 0;
}

void draw_pt_crosshair(V2 pt)
{
    const int expand = 4;
    const int line_extrude = 3;
    uint32_t crosshair_color = 0xa0a0a0ff;

    float minx = pt.x - expand;
    float miny = pt.y - expand;
    float maxx = pt.x + expand;
    float maxy = pt.y + expand;

    // BOX 
    draw_line((V2) { minx, miny }, (V2) { maxx, miny }, crosshair_color);
    draw_line((V2) { maxx, miny }, (V2) { maxx, maxy }, crosshair_color);
    draw_line((V2) { maxx, maxy }, (V2) { minx, maxy }, crosshair_color);
    draw_line((V2) { minx, maxy }, (V2) { minx, miny }, crosshair_color);

    // LINES
    draw_line((V2) { pt.x, miny - line_extrude }, (V2) { pt.x, miny + line_extrude }, crosshair_color);
    draw_line((V2) { pt.x, maxy - line_extrude }, (V2) { pt.x, maxy + line_extrude }, crosshair_color);
    draw_line((V2) { minx - line_extrude, pt.y }, (V2) { minx + line_extrude, pt.y }, crosshair_color);
    draw_line((V2) { maxx - line_extrude, pt.y }, (V2) { maxx + line_extrude, pt.y }, crosshair_color);
}

void free_editor(Editor *ed){
    free(ed->walls);
}

int double_wall_capacity(Editor *ed)
{
    ed->walls_capacity *= 2;
    ed->walls = realloc(ed->walls, ed->walls_capacity * sizeof(Wall));
}

int is_points_near(V2 pt1, V2 pt2, int tolerance)
{
    float dx = pt2.x - pt1.x;
    float dy = pt2.y - pt1.y;

    return (dx * dx + dy * dy < tolerance * tolerance);
}

V2 get_mouse_xy()
{
    Vector2 m = GetMousePosition();

    return (V2) { m.x, m.y };
}

int main(int argc, char *argv[])
{
    InitWindow(W_WIDTH, W_HEIGHT, W_TITLE);
    SetTargetFPS(60);

    printf("\n%d\n", argc);

    Editor ed;
    init_editor(&ed);

    int selected_method = EDITOR_DRAW_WALL;

    V2 mouse;

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);

        mouse = get_mouse_xy();

        int hovered = 0;
        
        for (int i = 0; i < ed.wall_count; ++i) {
            wall_display(ed.walls[i]);
            for (int j = 0; j < 2; j++){
                if (is_points_near(ed.walls[i].baseline[j], mouse, 10)){
                    draw_pt_crosshair(ed.walls[i].baseline[j]);
                    hovered = 1;
                    if (ed.state == EDITOR_NONE && IsMouseButtonPressed(0)){
                        ed.current_index = i;
                        ed.editing_index = j;
                        ed.state = EDITOR_EDIT_WALL;
                    }
                }
            }
        }

        // We are moving the point
        if (ed.state == EDITOR_EDIT_WALL || ed.state == EDITOR_DRAW_WALL){
            wall_update_point(&ed.walls[ed.current_index], mouse, ed.editing_index);
        }
        
        if (!IsMouseButtonDown(0)){
            ed.state = EDITOR_NONE;
        }

        if (ed.state == EDITOR_NONE && IsMouseButtonPressed(0) && !hovered){
            if (ed.wall_count == ed.walls_capacity) double_wall_capacity(&ed);
            ed.state = selected_method;
            ed.current_index = ed.wall_count;
            ed.editing_index = 1;
            wall_init(&ed.walls[ed.wall_count++], mouse, mouse, 10); 
        }

        if (IsKeyPressed(KEY_S)){
            write_level(&ed, "level.lvl");
        }

        EndDrawing();
    }

    free_editor(&ed);

    return 0;
}
