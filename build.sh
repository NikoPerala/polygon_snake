#!/bin/sh

set -xe

FILES="./src/drawfuncs.c 
       ./src/item.c
       ./src/level_parser.c 
       ./src/polygon.c
       ./src/snake.c 
       ./src/v2.c
       ./src/wall.c
       ./src/egfx.c
      "

#gcc $FILES ./src/main.c -lraylib -lm -o snakegame
gcc $FILES ./src/editor.c -lraylib -lm -o editor

