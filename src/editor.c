#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <raylib.h>

#include "snakegame.h"
#include "v2.h"
#include "egfx.h"
#include "editor.h"

#define POINTSYSTEM_CAPACITY 10


uint8_t init_editor(Editor *ed){
    ed->state = EDITOR_STATE_NONE;
    ed->pointsystem_id = -1;
    ed->point_id = -1;
    ed->pointsystem_capacity = POINTSYSTEM_CAPACITY;
    ed->hovered = 0;

    ed->pointsystems = (PointSystem*) malloc(sizeof(PointSystem) * ed->pointsystem_capacity);
    if (ed->pointsystems == NULL) return 1;
    for (int i = 0; i < ed->pointsystem_capacity; ++i){
        ed->pointsystems[i] = (PointSystem) { 0 };
    }

    add_pointsystem(ed, ARROW);
    ed->pointsystems[0].points[0] = (V2) { W_WIDTH >> 1 , W_HEIGHT >> 1 };
    update_pointsystem(&ed->pointsystems[0]);
    // First pointsystem is the startpoint
    /*
    ed->pointsystem_count = 1;
    if (initialize_pointsystem(&ed->pointsystems[0], ARROW) != 0) return 1;
    ed->pointsystems[0].points[0] = (V2) { W_WIDTH >> 1 , W_HEIGHT >> 1 };
    update_pointsystem(&ed->pointsystems[0]);
    */

    return 0;
}

uint8_t add_pointsystem(Editor *ed, PointSystemType type)
{
    if (ed->pointsystem_count >= ed->pointsystem_capacity)
    {
        ed->pointsystem_capacity *= 2;
        ed->pointsystems = (PointSystem*) realloc(ed->pointsystems, sizeof(PointSystem) * ed->pointsystem_capacity);
    }

    initialize_pointsystem(&ed->pointsystems[ed->pointsystem_count], type);
    ed->pointsystem_count++;
}

uint8_t initialize_pointsystem(PointSystem *ps, PointSystemType type)
{
    ps->type = type;
    switch (ps->type){
        case WALL:
            ps->point_amount = 6;
            ps->movable_points = 2;
            break;
        case ARROW:
            ps->point_amount = 4;
            ps->movable_points = 1;
            break;
    }
    ps->angle = 0;
    ps->points = (V2*) malloc(sizeof(V2) * ps->point_amount);
    if (ps->points == NULL) return 1;
    for (int i = 0; i < ps->point_amount; ++i){
        ps->points[i] = (V2) { 0 };
    }
    
    return 0;
}


void display_pointsystem(eCanvas *canvas, PointSystem *ps)
{
    switch (ps->type){
        case WALL:
            display_wall(canvas, ps, 0xff00A000);
            break;
        case ARROW:
            display_arrow(canvas, ps, 0xFFA0A0A0);
            break;
    }
}
void update_pointsystem(PointSystem *ps)
{
    switch (ps->type){
        case WALL:
            update_wall(ps);
            break;
        case ARROW:
            update_arrow(ps->points, ps->angle);
            break;
    }
}

void set_pointsystem_point(PointSystem *ps, int id, V2 pt)
{
    ps->points[id] = pt;
}

int count_pointsystem_type(Editor *ed, PointSystemType type)
{
    int retval = 0;

    for (int i = 0; i < ed->pointsystem_count; ++i)
    {
        if (ed->pointsystems[i].type == type) retval++;
    }

    return retval;
}

int write_level(Editor *ed, char *name)
{

    FILE *fp;
    char row[20];

    fp = fopen(name, "w");
    if (fp == NULL) return 1;

    fprintf(fp, "/\n");
    sprintf(row, "W%d\n", count_pointsystem_type(ed, WALL));
    fprintf(fp, row);

    PointSystem *ps;

    for (int i = 0; i < ed->pointsystem_count; ++i)
    {
        ps = &ed->pointsystems[i];
        switch (ps->type)
        {
            case WALL:
                float x1 = ps->points[0].x / W_WIDTH;
                float y1 = ps->points[0].y / W_HEIGHT; 
                float x2 = ps->points[1].x / W_WIDTH;
                float y2 = ps->points[1].y / W_HEIGHT;
                sprintf(row, "w%.3f,%.3f:%.3f,%.3f\n", x1, y1, x2, y2);
                break;
            case ARROW:
                float x = ps->points[0].x / W_WIDTH;
                float y = ps->points[0].y / W_WIDTH;
                sprintf(row, "s%.3f,%.3f:%.3f\n", x, y, ps->angle);
                break;
        }
        fprintf(fp, row);
    }
/*
    for (int i = 0; i < ed->wall_count; ++i){
        float x1 = ed->walls[i].baseline[0].x / W_WIDTH;
        float y1 = ed->walls[i].baseline[0].y / W_HEIGHT;
        float x2 = ed->walls[i].baseline[1].x / W_WIDTH;
        float y2 = ed->walls[i].baseline[1].y / W_HEIGHT;
        sprintf(row, "w%.3f,%.3f:%.3f,%.3f\n", x1, y1, x2, y2);
        fprintf(fp, row);
    }
*/
    fclose(fp);

    return 0;
}

void draw_pt_crosshair(eCanvas *canvas, V2 pt)
{
    const int expand = 4;
    const int line_extrude = 3;
    uint32_t crosshair_color = 0xa0a0a0ff;

    float minx = pt.x - expand;
    float miny = pt.y - expand;
    float maxx = pt.x + expand;
    float maxy = pt.y + expand;

    eDrawLine(canvas, minx, miny, maxx, miny, crosshair_color);
    eDrawLine(canvas, maxx, miny, maxx, maxy, crosshair_color);
    eDrawLine(canvas, maxx, maxy, minx, maxy, crosshair_color);
    eDrawLine(canvas, minx, maxy, minx, miny, crosshair_color);

    eDrawLine(canvas, pt.x, miny - line_extrude, pt.x, miny + line_extrude, crosshair_color);
    eDrawLine(canvas, pt.x, maxy - line_extrude, pt.x, maxy + line_extrude, crosshair_color);
    eDrawLine(canvas, minx - line_extrude, pt.y, minx + line_extrude, pt.y, crosshair_color);
    eDrawLine(canvas, maxx - line_extrude, pt.y, maxx + line_extrude, pt.y, crosshair_color);
    
}

void free_editor(Editor *ed)
{
    for (int i = 0; i < ed->pointsystem_count; ++i){
        free(ed->pointsystems[i].points);
    }

    free(ed->pointsystems);
}

int double_pointsystem_capacity(Editor *ed)
{
    ed->pointsystem_capacity *= 2;
    ed->pointsystems = realloc(ed->pointsystems, ed->pointsystem_capacity * sizeof(PointSystem));
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

#define ARROW_LENGTH 30
#define ARROW_POINTER_LENGTH 10
#define ARROW_ANGLE QUARTERPI

void display_wall(eCanvas *canvas, PointSystem *ps, uint32_t color)
{
    V2 points[4] = {
        ps->points[2],
        ps->points[3],
        ps->points[4],
        ps->points[5],
    };

    eFillPolygon(canvas, points, 4, color);
}

void update_wall(PointSystem *ps)
{
    float dx = ps->points[1].x - ps->points[0].x;
    float dy = ps->points[1].y - ps->points[0].y;

    ps->angle = atan2(dy, dx) + HALFPI;

    ps->points[2].x = ps->points[0].x + WALL_THICKNESS * cos(ps->angle);
    ps->points[2].y = ps->points[0].y + WALL_THICKNESS * sin(ps->angle);

    ps->points[3].x = ps->points[1].x + WALL_THICKNESS * cos(ps->angle);
    ps->points[3].y = ps->points[1].y + WALL_THICKNESS * sin(ps->angle);

    ps->points[4].x = ps->points[1].x + WALL_THICKNESS * cos(ps->angle + PI);
    ps->points[4].y = ps->points[1].y + WALL_THICKNESS * sin(ps->angle + PI);

    ps->points[5].x = ps->points[0].x + WALL_THICKNESS * cos(ps->angle + PI);
    ps->points[5].y = ps->points[0].y + WALL_THICKNESS * sin(ps->angle + PI);
}

void update_arrow(V2 *points, float angle)
{ 
    points[1] = (V2){
            points[0].x + ARROW_LENGTH * cos(angle),
            points[0].y + ARROW_LENGTH * sin(angle)
                    };
    points[2] = (V2) {
            points[1].x + ARROW_POINTER_LENGTH * cos(angle - THREEQUARTERPI),
            points[1].y + ARROW_POINTER_LENGTH * sin(angle - THREEQUARTERPI)
                           };
    points[3] = (V2) {
            points[1].x + ARROW_POINTER_LENGTH * cos(angle + THREEQUARTERPI),
            points[1].y + ARROW_POINTER_LENGTH * sin(angle + THREEQUARTERPI)
                           };
}

void display_arrow(eCanvas *canvas, PointSystem *ps, uint32_t color)
{
    eDrawLine(canvas, 
              ps->points[0].x, ps->points[0].y,
              ps->points[1].x, ps->points[1].y, color);
    eDrawLine(canvas, 
              ps->points[1].x, ps->points[1].y,
              ps->points[2].x, ps->points[2].y, color);
    eDrawLine(canvas, 
              ps->points[1].x, ps->points[1].y,
              ps->points[3].x, ps->points[3].y, color);
}

int main(int argc, char *argv[])
{
    InitWindow(W_WIDTH, W_HEIGHT, W_TITLE);
    SetTargetFPS(60);
    
    eCanvas canvas;
    eInitializeCanvas(&canvas, W_WIDTH, W_HEIGHT, 0x0);


    Image img = (Image) { canvas.pixels, W_WIDTH, W_HEIGHT, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
    Texture2D tex = LoadTextureFromImage(img);

    Editor ed;
    init_editor(&ed);

    V2 mouse;

    char dbgmsg[64];

    while(!WindowShouldClose()){
        BeginDrawing();
        eFillCanvas(&canvas, 0xff181818);
        mouse = get_mouse_xy();

        if (ed.state != EDITOR_STATE_NONE && !IsMouseButtonDown(0))
        {
            ed.state = EDITOR_STATE_NONE;
        }


        for (int i = 0; i < ed.pointsystem_count; ++i){
            for (int pid = 0; pid < ed.pointsystems[i].movable_points; ++pid)
            {
                ed.hovered = 0;
                if (is_points_near(ed.pointsystems[i].points[pid], mouse, 10) && ed.state == EDITOR_STATE_NONE)
                {
                    ed.hovered = 1;
                    draw_pt_crosshair(&canvas, ed.pointsystems[i].points[pid]);
                    if (IsMouseButtonPressed(0))
                    {
                        ed.pointsystem_id = i;
                        ed.point_id = pid;
                        ed.state = EDITOR_STATE_MOVE_POINT;
                    }
                }
            }
            display_pointsystem(&canvas, &ed.pointsystems[i]);
        }

        if (ed.state == EDITOR_STATE_NONE && IsMouseButtonPressed(0) && !ed.hovered)
        {
            add_pointsystem(&ed, WALL);
            ed.pointsystems[ed.pointsystem_count - 1].points[0] = mouse;
            ed.pointsystems[ed.pointsystem_count - 1].points[1] = mouse;
            update_pointsystem(&ed.pointsystems[ed.pointsystem_count - 1]);
            ed.state = EDITOR_STATE_MOVE_POINT;
            ed.pointsystem_id = ed.pointsystem_count - 1;
            ed.point_id = 1;            
        }

        switch (ed.state){
            case EDITOR_STATE_MOVE_POINT:
                ed.pointsystems[ed.pointsystem_id].points[ed.point_id] = mouse;
                update_pointsystem(&ed.pointsystems[ed.pointsystem_id]);
                break;
        }


        #define ROTATE 0.05
        if (IsKeyDown(KEY_LEFT)){
            ed.pointsystems[0].angle -= ROTATE;
            update_pointsystem(&ed.pointsystems[0]);
        }
        if (IsKeyDown(KEY_RIGHT)){
            ed.pointsystems[0].angle += ROTATE;
            update_pointsystem(&ed.pointsystems[0]);
        }
                
        if (IsKeyPressed(KEY_S)){
            write_level(&ed, "level.lvl");
        }
        UpdateTexture(tex, canvas.pixels);
        DrawTexture(tex, 0, 0, WHITE);
        sprintf(dbgmsg, "startpos x: %.2f y: %.2f angle: %.2f", ed.pointsystems[0].points[0].x, ed.pointsystems[0].points[0].y,  ed.pointsystems[0].angle);
        DrawText(dbgmsg, 20, 20, 20, WHITE);
        EndDrawing();
    }

    free_editor(&ed);

    return 0;
}
