#!/bin/sh

set -xe

FILES="
       ./src/item.c
       ./src/level_parser.c 
       ./src/polygon.c
       ./src/snake.c 
       ./src/v2.c
       ./src/wall.c
       ./src/egfx.c
       ./src/pointsystem.c
       ./src/common.c
      "

#gcc $FILES ./src/main.c -lraylib -lm -g -o snakegame
gcc $FILES ./src/editor.c -lraylib -lm -o editor

