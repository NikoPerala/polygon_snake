#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <raylib.h>
#include <raymath.h>

#include "egfx.h"
#include "snakegame.h"

int main(int argc, char *argv[]){

    InitWindow(W_WIDTH, W_HEIGHT, W_TITLE);
    SetTargetFPS(60);

    srand(time(NULL));

    eCanvas canvas;
    eInitializeCanvas(&canvas, W_WIDTH, W_HEIGHT, 0xff000000);

    Color background_color = BACKGROUND_COLOR;

   
    Level level = get_level(W_WIDTH, W_HEIGHT);  
    create_level_from_file("level.lvl", &level);
    
    printf("Created level with %d walls.\n", level.wall_amount);

    Snake snake = { 0 };
    init_snake(&snake);
    snake_reset(&snake, level.startpoint, level.startangle);

    Item food = { 0 };
    food.type = ITEM_APPLE;
    initialize_item(&food);
    food.position = V2_random(0, W_WIDTH, 0, W_HEIGHT);

    char dbg_msg[100];

    char game_stats[64];

    int game_state = GAME_ON;
    int score = 0;
    int highscore = 0;
    int reset = 0;

    Image img = (Image) { canvas.pixels, W_WIDTH, W_HEIGHT, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };
    Texture2D tex = LoadTextureFromImage(img);

    while (!WindowShouldClose()){
        eFillCanvas(&canvas, 0xff181818);
        //
        // GAME LOOP 
        //

        if (!snake_move(&snake)) reset = 1; 

        if (check_collision_polygon(snake.headpoints, 4, food.points, food.point_amount)){
            food.position = V2_random(0, W_WIDTH, 0, W_HEIGHT);
            if (snake_eat(&snake)) break;
            score++;
        }
       

        for (int i = 0; i < level.wall_amount; ++i){
            wall_display(&canvas, level.walls[i]);
            if (snake.members[0].teleportation == 0 && check_collision_polygon(level.walls[i].points, 4, snake.headpoints, 4)) {
                reset = 1; 
            }    
        }

        snake_display(&canvas, &snake);
        display_item(&canvas, &food);
        food.angle += 0.05;


        BeginDrawing();
        UpdateTexture(tex, canvas.pixels);
        DrawTexture(tex, 0, 0, WHITE);

        if (IsKeyDown(KEY_LEFT)) {
            snake.direction -= SNAKE_ROTATION_SPEED;
            if (snake.direction < 0) snake.direction += TAU;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            snake.direction += SNAKE_ROTATION_SPEED;
            if (snake.direction > TAU) snake.direction -= TAU;
        }

        if (reset) {
            reset = 0;
            snake_reset(&snake, level.startpoint, level.startangle);
            highscore = score > highscore ? score : highscore;
            score = 0;
        }

        sprintf(game_stats, "Score: %d Highscore: %d", score, highscore);
        DrawText(game_stats, 20, 20, 20, GRAY);

        EndDrawing();
    }

    CloseWindow();

    free_level(&level);
    free_item(&food);
    free_snake(&snake);
    
    eFreeCanvas(&canvas);

    return 0;
}
