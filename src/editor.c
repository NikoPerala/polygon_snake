#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#include <raylib.h>
#undef PI

#include "snakegame.h"
#include "v2.h"
#include "egfx.h"
#include "editor.h"
//#include "editor_funcs.h"


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

    //First pointsystem is the startpoint
    add_pointsystem(ed, ARROW);
    ed->pointsystems[0].points[0] = (V2) { W_WIDTH >> 1 , W_HEIGHT >> 1 };
    update_pointsystem(&ed->pointsystems[0]);

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
                sprintf(row, "w%f,%f:%f,%f\n", x1, y1, x2, y2);
                break;
            case ARROW:
                float x = ps->points[0].x / W_WIDTH;
                float y = ps->points[0].y / W_WIDTH;
                sprintf(row, "s%f,%f:%f\n", x, y, ps->angle);
                break;
        }
        fprintf(fp, row);
    }
    
    fclose(fp);

    return 0;
}

void draw_pt_crosshair(eCanvas *canvas, V2 pt)
{
    const int expand = 4;
    const int line_extrude = 3;
    uint32_t crosshair_color = 0xFFDDDDDD;

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
    V2 d = V2_delta_vector(pt1, pt2);
    
    return (d.x * d.x + d.y * d.y < tolerance * tolerance);
}

V2 get_mouse_xy()
{
    Vector2 m = GetMousePosition();
    return (V2) { m.x, m.y };
}

float distance_from_line(V2 pt, V2 pt1, V2 pt2)
{
    V2 d = V2_delta_vector(pt1, pt2);

    float num = abs((pt2.x - pt1.x) * (pt1.y - pt.y) - (pt1.x - pt.x) * (pt2.y - pt1.y));
    float den = sqrtf(d.x * d.x + d.y * d.y);
    
    return num / den;
}

void delete_pointsystem(Editor *ed, int id)
{
    for (int i = id; i < ed->pointsystem_count - 1; ++i){
        ed->pointsystems[i] = ed->pointsystems[i + 1];
    }
    ed->pointsystem_count--;
}

#define DEBUG_W_WIDTH 450
void draw_debug(Editor *ed)
{
    DrawRectangle(W_WIDTH, 0, DEBUG_W_WIDTH, W_HEIGHT, BLACK);

    Color color = (Color) { 0xA0, 0xA0, 0xA0, 0xff };
    char dbgmsg[128];
    int font_size = 20;
    
    sprintf(dbgmsg, "Editor:");
    DrawText(dbgmsg, W_WIDTH + 5, 0 * font_size + 5, font_size, color);

    sprintf(dbgmsg, "  state: %s", EditorStateNames[ed->state]);
    DrawText(dbgmsg, W_WIDTH + 5, 1 * font_size + 5, font_size, color);

    sprintf(dbgmsg, "  PS Capacity: %d", ed->pointsystem_capacity);
    DrawText(dbgmsg, W_WIDTH + 5, 2 * font_size + 5, font_size, color);
    
    sprintf(dbgmsg, "  PS Count: %d", ed->pointsystem_count);
    DrawText(dbgmsg, W_WIDTH + 5, 3 * font_size + 5, font_size, color);
    
    sprintf(dbgmsg, "  PS id: %d", ed->pointsystem_id);
    DrawText(dbgmsg, W_WIDTH + 5, 4 * font_size + 5, font_size, color);
    
    sprintf(dbgmsg, "  point id: %d", ed->point_id);
    DrawText(dbgmsg, W_WIDTH + 5, 5 * font_size + 5, font_size, color);
}

void reset_editor_state(Editor *ed)
{
    ed->state = EDITOR_STATE_NONE;
    ed->pointsystem_id = -1;
    ed->point_id = -1;
    ed->state_ready = 0;
}

int main(int argc, char *argv[])
{
    InitWindow(W_WIDTH + DEBUG_W_WIDTH, W_HEIGHT, W_TITLE);
    SetTargetFPS(60);
    
    eCanvas canvas;
    eInitializeCanvas(&canvas, W_WIDTH, W_HEIGHT, 0x0);
    
    Image img = (Image) { canvas.pixels, W_WIDTH, W_HEIGHT, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
    Texture2D tex = LoadTextureFromImage(img);
    
    Editor ed;
    init_editor(&ed);

    V2 mouse;

    while(!WindowShouldClose()){
        BeginDrawing();
        eFillCanvas(&canvas, 0xff181818);
        
        mouse = get_mouse_xy();


        if (ed.state != EDITOR_STATE_NONE && !IsMouseButtonDown(0)) reset_editor_state(&ed);

        for (int i = 0; i < ed.pointsystem_count; ++i){
            /*
             * MOVE POINT IN POINTSYSTEM
             */
            for (int pid = 0; pid < ed.pointsystems[i].movable_points; ++pid)
            {
                if (is_points_near(ed.pointsystems[i].points[pid], mouse, 10) && ed.state == EDITOR_STATE_NONE)
                {
                    draw_pt_crosshair(&canvas, ed.pointsystems[i].points[pid]);
                    
                    if (IsMouseButtonPressed(0))
                    {
                        ed.point_id = pid;
                        ed.pointsystem_id = i;
                        ed.state = EDITOR_STATE_MOVE_POINT;
                    }
                }
            }


            /*
             * MOVE WHOLE POINTSYSTEM HANDLING
             */
            uint32_t color = 0xFF00A000;
            //if (ed.pointsystems[i].type == WALL && ed.point_id < 0 && ed.state == EDITOR_STATE_NONE) 
            if (ed.point_id < 0 && ed.state == EDITOR_STATE_NONE) 
            {
                PointSystem *ps = &ed.pointsystems[i];
                switch (ps->type){
                    
                    case WALL:
                        float distance = distance_from_line(mouse, ps->points[0], ps->points[1]);
                        V2 min = V2_min(ps->points[0], ps->points[1]);
                        V2 max = V2_max(ps->points[0], ps->points[1]);

                        float min_x = ps->points[0].x < ps->points[1].x ? ps->points[0].x : ps->points[1].x;
                        float min_y = ps->points[0].y < ps->points[1].y ? ps->points[0].y : ps->points[1].y;
                        float max_x = ps->points[0].x > ps->points[1].x ? ps->points[0].x : ps->points[1].x;
                        float max_y = ps->points[0].y > ps->points[1].y ? ps->points[0].y : ps->points[1].y;

                        int close = (distance < 5 &&
                                     mouse.x > min_x && mouse.x < max_x &&
                                     mouse.y > min_y && mouse.y < max_y);
/*
                        int close = (distance < 5 &&
                                     mouse.x > min.x && mouse.x < max.x &&
                                     mouse.y > min.x && mouse.y < max.y);
                       */ 
                        color = close ? 0xFF90A090 : 0xFF00A000;
                        if (IsMouseButtonPressed(0) && close){
                            ed.state = EDITOR_STATE_MOVE_WALL;
                            ed.pointsystem_id = i;
                        } if (IsMouseButtonPressed(1) && close){
                            delete_pointsystem(&ed, i);
                        }
                        break;
                }
            }
            display_pointsystem(&canvas, &ed.pointsystems[i], color);
        }

        if (ed.state == EDITOR_STATE_NONE && IsMouseButtonPressed(0) && ed.point_id < 0)
        {
            add_pointsystem(&ed, WALL);
            ed.pointsystems[ed.pointsystem_count - 1].points[0] = mouse;
            ed.pointsystems[ed.pointsystem_count - 1].points[1] = mouse;
            update_pointsystem(&ed.pointsystems[ed.pointsystem_count - 1]);
            ed.state = EDITOR_STATE_MOVE_POINT;
            ed.pointsystem_id = ed.pointsystem_count - 1;
            ed.point_id = 1;            
        }

        /*
         *  EDITOR STATE HANDLING
         */
        switch (ed.state){
            case EDITOR_STATE_MOVE_POINT:
                V2 *pt = &ed.pointsystems[ed.pointsystem_id].points[ed.point_id];
                
                *pt = mouse;
                update_pointsystem(&ed.pointsystems[ed.pointsystem_id]);
                draw_pt_crosshair(&canvas, *pt);
                break;
            case EDITOR_STATE_MOVE_WALL:
                PointSystem *ps = &ed.pointsystems[ed.pointsystem_id];
                move_wall(ps, mouse, &ed.state_ready);
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

        draw_debug(&ed);

        EndDrawing();
    }

    free_editor(&ed);

    return 0;
}
