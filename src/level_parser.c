#include <stdio.h>
#include <stdint.h>
#include "snakegame.h"
#include <string.h>

#define GOT_WALL_AMOUNT 0

typedef enum {
    LEVEL_STATUS_OK,
    LEVEL_STATUS_ALLOCATION_ERROR,
    LEVEL_FILE_OPEN_ERROR,
    LEVEL_INVALID_FILE_ERROR
} LEVEL_STATUSES;

Level get_level(int w, int h) //quick && dirty
{
    Level lvl;
    lvl.loading_status = 0;
    lvl.wall_count = 0;
    lvl.screen_width = w;
    lvl.screen_height = h;
    return lvl;
}


void free_level(Level *level)
{
    free(level->walls);
}

uint8_t process_line(char *line, Level *level)
{
    switch (line[0]){
        case '/':
            // TODO: NOT IMPLEMENTED: New level
            return 0;
        case 's':

            float x, y, angle;
            sscanf(line, "s%f,%f:%f", &x, &y, &angle);

            V2 pt = (V2) { x * level->screen_width, y * level->screen_height };
            level->startpoint = pt;
            level->startangle = angle;
            break;
        case 'w':
            if (!(level->loading_status && (1 << GOT_WALL_AMOUNT))) return LEVEL_INVALID_FILE_ERROR;
            // Parse coords
            float x1, y1, x2, y2;
            
            sscanf(line, "w%f,%f:%f,%f", &x1, &y1, &x2, &y2);
            
            V2 pt1 = { x1 * (float) level->screen_width,(float) y1 * level->screen_height };             
            V2 pt2 = { x2 * (float) level->screen_width,(float) y2 * level->screen_height };             

            wall_init(&level->walls[level->wall_count++], pt1, pt2, WALL_THICKNESS);
                   /* level->wall_count - 1, 
                    pt1.x, 
                    pt1.y,
                    pt2.x,
                    pt2.y);*/
            break;
        case 'W':
            level->wall_amount = (atoi(line + 1));
            
            level->walls = malloc(sizeof(Wall) * level->wall_amount);
            if (level->walls == NULL) return 1;

            for (int i = 0; i < level->wall_amount; ++i){
                level->walls[i] = (Wall) { 0 };
            }

            level->loading_status |= (1 << GOT_WALL_AMOUNT);
            break;
        case 'N':
            //TODO: ("NOT IMPLEMENTED: Level name
            return 0;
    }

}

uint8_t create_level_from_file(const char *path, Level *level)
{
    FILE *fp;

    char c;

    fp = fopen(path, "r");
    if (fp == NULL) return LEVEL_FILE_OPEN_ERROR;

    char buffer[64];
    uint8_t buffer_count = 0;
    while ((c = getc(fp)) != EOF){
        if (c == '\n') {
            buffer[buffer_count++] = '\0';
            process_line(buffer, level);
            buffer_count = 0;
        } else {
            buffer[buffer_count++] = c;
        }
    }

    return 0;
}
