#include "editor_funcs.h"
#include "snakegame.h"

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

