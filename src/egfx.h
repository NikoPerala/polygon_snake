#ifndef EPHO_GFX_H
#define EPHO_GFX_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "v2.h"
/*
typedef struct V2 {
    float x, y;
} V2;
*/
typedef struct eCanvas {
  int width;
  int height;
  uint32_t size;
  uint32_t *pixels;
} eCanvas;


void eFillCanvas(eCanvas *c, uint32_t color);
eCanvas eGetCanvas(int width, int height, uint32_t color);
uint8_t eInitializeCanvas(eCanvas *canvas, int width, int height, uint32_t color);
void eWriteCanvasToPpm(eCanvas *c, const char *filepath);
void eFreeCanvas(eCanvas *c);
void eDrawLineHorizontal(eCanvas *c, int x1, int x2, int y, uint32_t color);
void eDrawRectangle(eCanvas *c, int ox, int oy, int w, int h, uint32_t color);
void eDrawCircle(eCanvas *c, int ox, int oy, int r, uint32_t color);
void eDrawCircleBorder(eCanvas *c, int ox, int oy, int r, int t, uint32_t color);
void eFillPolygon(eCanvas *c, V2 *points, int point_amount, uint32_t color);
void eDrawPolygon(eCanvas *c, V2 *points, int point_amount, uint32_t color);


//
//  Line functions
//
void ePlotLineLow(eCanvas *c, 
                  int x1, int y1, 
                  int x2, int y2, 
                  uint32_t color);
void ePlotLineHigh(eCanvas *c, 
                  int x1, int y1, 
                  int x2, int y2, 
                  uint32_t color);
void eDrawLine(eCanvas *c, 
              int x1, int y1, 
              int x2, int y2, 
              uint32_t color);

//
// Triangle functions
//
void eFillBottomFlatTriangle(eCanvas *c,
                            int x1, int y1,
                            int x2, int y2,
                            int x3, int y3,
                            uint32_t color);
void eFillTopFlatTriangle(eCanvas *c,
                         int x1, int y1,
                         int x2, int y2,
                         int x3, int y3,
                         uint32_t color);
void eDrawTriangle(eCanvas *c,
                  int x1, int y1,
                  int x2, int y2,
                  int x3, int y3,
                  uint32_t color);

void eInvertRectangle(eCanvas *c, int ox, int oy, int w, int h);
void eInvertPixel(eCanvas *c, int x, int y);
void eInvertCircleBorder(eCanvas *c, int ox, int oy, int r, int t);
void eInvertCircle(eCanvas *c, int ox, int oy, int r);

uint8_t lerpU8(uint8_t a, uint8_t b, float pos);
uint32_t eLerpColor(uint32_t c1, uint32_t c2, float pos);

#endif // EPHO_GFX_H
