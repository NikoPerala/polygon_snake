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
       ./src/main.c"

gcc $FILES -lraylib -lm -g -o snakegame
#gcc $FILES -lraylib -lm -g ./src/editor.c -o editor

