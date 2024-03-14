#include "egfx.h"

void eFillCanvas(eCanvas *c, uint32_t color){
    int i;
    for (i=0; i<c->size; ++i){
      c->pixels[i] = color;
    }
}

eCanvas eGetCanvas(int width, int height, uint32_t color)
{
  eCanvas canvas;
  eInitializeCanvas(&canvas, width, height, color);
  return canvas;
}


uint8_t eInitializeCanvas(eCanvas *canvas, int width, int height, uint32_t color)
{
  canvas->width = width;
  canvas->height = height;
  canvas->size = width * height;
  canvas->pixels = malloc(sizeof(uint32_t) * canvas->size);
  if (canvas->pixels == NULL) return 1;

  eFillCanvas(canvas, color);

  return 0;
}

void eWriteCanvasToPpm(eCanvas *c, const char *filepath)
{
  int i;
  FILE *fp = fopen(filepath, "wb+");
  char tmp[100];

  uint8_t red;
  uint8_t green;
  uint8_t blue;

  fprintf(fp, "P6\n");
  sprintf(tmp, "%d %d\n", c->width, c->height);
  fprintf(fp, tmp);
  fprintf(fp, "255\n");

  for (i = 0; i < c->size; ++i){
    red = (c->pixels[i] >> (2 * 8)) & 0xff;
    green = (c->pixels[i] >> (1 * 8)) & 0xff;
    blue = (c->pixels[i] >> (0 * 8)) & 0xff;
    fwrite((const void*)&red, sizeof(uint8_t), 1, fp);
    fwrite((const void*)&green, sizeof(uint8_t), 1, fp);
    fwrite((const void*)&blue, sizeof(uint8_t), 1, fp);
  }

  fclose(fp);
}

void eFreeCanvas(eCanvas *c){
  free(c->pixels);
}


void eDrawLineHorizontal(eCanvas *c, int x1, int x2, int y, uint32_t color)
{
  int i;
  int length = x2 - x1;

  if (length < 0){
    int tmp;
    tmp = x2;
    x2 = x1;
    x1 = tmp;
  }

  for (i = x1; i < x2; ++i){
    c->pixels[y * c->width + i] = color;
  }
}

void eDrawRectangle(eCanvas *c, int ox, int oy, int w, int h, uint32_t color)
{
  int y;

  for (y = oy; y < oy + h; ++y){
    eDrawLineHorizontal(c, ox, ox + w, y,  color);
  }
}

void eDrawCircle(eCanvas *c, int ox, int oy, int r, uint32_t color)
{
    int min_x = ox - r;
    int max_x = ox + r;
    int min_y = oy - r;
    int max_y = oy + r;

    int y, x;

    for (y = min_y; y < max_y; ++y){
        for (x = min_x; x < max_x; ++x){
            int cx = x - ox;
            int cy = y - oy;
            if (cx * cx + cy * cy <= r * r){
                if (y < 0 || y >= c->height || x < 0 || x >= c->width) continue;
                c->pixels[y * c->width + x] = color;
            }
        }
    }
}

void eDrawCircleBorder(eCanvas *c, int ox, int oy, int r, int t, uint32_t color)
{

    int min_x = ox - r;
    int max_x = ox + r;
    int min_y = oy - r;
    int max_y = oy + r;

    int y, x;
    for (y = min_y; y < max_y; ++y){
        for (x = min_x; x < max_x; ++x){
            int sdx = (x - ox) * (x - ox);
            int sdy = (y - oy) * (y - oy);
            if (sdx + sdy <= r * r
             && sdx + sdy > (r - t) * (r - t)){
                if (y < 0 || y >= c->height || x < 0 || x >= c->width) continue;
                c->pixels[y * c->width + x] = color;
            }
        }
    }
}

void eInvertCircleBorder(eCanvas *c, int ox, int oy, int r, int t)
{

    int min_x = ox - r;
    int max_x = ox + r;
    int min_y = oy - r;
    int max_y = oy + r;

    int y, x;
    for (y = min_y; y < max_y; ++y){
        for (x = min_x; x < max_x; ++x){
            int sdx = (x - ox) * (x - ox);
            int sdy = (y - oy) * (y - oy);
            if (sdx + sdy <= r * r
             && sdx + sdy > (r - t) * (r - t)){
                if (y < 0 || y >= c->height || x < 0 || x >= c->width) continue;
                eInvertPixel(c, x, y);
            }
        }
    }
}

/*
 *
 *
 */


void ePlotLineLow(eCanvas *c, int x1, int y1, int x2, int y2, uint32_t color)
{
    int x;

    int yi = 1;
    int16_t dx = x2 - x1;
    int16_t dy = y2 - y1;
    if (dy < 0){
        yi = -1;
        dy = -dy;
    }
    int16_t D = (2 * dy) - dx;
    int y = y1;

    for (x = x1; x <= x2; ++x){
        c->pixels[y * c->width + x] = color;
        if (D > 0){
            y += yi;
            D += (2 * (dy - dx));
        } else {
            D += 2 * dy;
        }
    }
}

void ePlotLineHigh(eCanvas *c, int x1, int y1, int x2, int y2, uint32_t color)
{
    int y;
    int16_t dx = x2 - x1;
    int16_t dy = y2 - y1;
    int xi = 1;
    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }
    int16_t D = (2 * dx) - dy;
    int x = x1;

    for (y = y1; y <= y2; ++y){
        c->pixels[y * c->width + x] = color;
        if (D > 0){
            x += xi;
            D += (2 * (dx - dy));
        } else {
            D += 2 * dx;
        }
    }
}


void eDrawLine(eCanvas *c, int x1, int y1, int x2, int y2, uint32_t color)
{
    int16_t dx = x2 - x1;
    int16_t dy = y2 - y1;
    if (abs(dy) < abs(dx)) {
        if (x1 > x2) ePlotLineLow(c, x2, y2, x1, y1, color);
        else ePlotLineLow(c, x1, y1, x2, y2, color);

    } else {
        if (y1 > y2) ePlotLineHigh(c, x2, y2, x1, y1, color);
        else ePlotLineHigh(c, x1, y1, x2, y2, color);
    }
}

void eFillBottomFlatTriangle(eCanvas *c,
                            int x1, int y1,
                            int x2, int y2,
                            int x3, int y3,
                            uint32_t color)
{
    int scanlineY;

    double invslope1 = (double)(x2 - x1) / (double)(y2 - y1);
    double invslope2 = (double)(x3 - x1) / (double)(y3 - y1);

    double curx1 = (double)x1;
    double curx2 = (double)x1;

    for (scanlineY = y1; scanlineY <= y2; ++scanlineY){
        eDrawLineHorizontal(c, (int)curx1, (int)curx2, scanlineY, color);
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

void eFillTopFlatTriangle(eCanvas *c,
                         int x1, int y1,
                         int x2, int y2,
                         int x3, int y3,
                         uint32_t color)
{
    int scanlineY;
    double invslope1 = (double)(x3 - x1) / (double)(y3 - y1);
    double invslope2 = (double)(x3 - x2) / (double)(y3 - y2);

    double curx1 = (double)x3;
    double curx2 = (double)x3;

    for (scanlineY = y3; scanlineY > y1; --scanlineY){
        eDrawLineHorizontal(c, (int)curx1, (int)curx2, scanlineY, color);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

void eDrawTriangle(eCanvas *c,
                  int x1, int y1,
                  int x2, int y2,
                  int x3, int y3,
                  uint32_t color)
{
    int tmp;
    if (y2 < y1) {
      tmp = x1;
      x1 = x2;
      x2 = tmp;

      tmp = y1;
      y1 = y2;
      y2 = tmp;

    }
    if (y3 < y1) {
      tmp = y1;
      y1 = y3;
      y3 = tmp;

      tmp = x1;
      x1 = x3;
      x3 = tmp;
    }
    if (y3 < y2) {
      tmp = y2;
      y2 = y3;
      y3 = tmp;

      tmp = x2;
      x2 = x3;
      x3 = tmp;
    }

    if (y2 == y3) eFillBottomFlatTriangle(c, x1, y1, x2, y2, x3, y3, color);
    else if (y1 == y2) eFillTopFlatTriangle(c, x1, y1, x2, y2, x3, y3, color);
    else {
        int x4 = (int)(x1 + ((float)(y2 - y1) / (float)(y3 - y1)) * (x3 - x1));
        int y4 = y2;
        eFillBottomFlatTriangle(c, x1, y1, x2, y2, x4, y4, color);
        eFillTopFlatTriangle(c, x2, y2, x4, y4, x3, y3, color);
    }
}

void eInvertRectangle(eCanvas *c, int ox, int oy, int w, int h)
{
  int x, y, cPos;

  for (y = oy; y < oy + h; ++y){
    for (x = ox; x < ox + w; ++x){
      cPos = y * c->width + x;
      c->pixels[cPos] = 0xffffff ^ c->pixels[cPos];
    }
  }
}

void eInvertCircle(eCanvas *c, int ox, int oy, int r)
{
    int min_x = ox - r;
    int max_x = ox + r;
    int min_y = oy - r;
    int max_y = oy + r;

    int y, x;

    for (y = min_y; y < max_y; ++y){
        for (x = min_x; x < max_x; ++x){
            int cx = x - ox;
            int cy = y - oy;
            if (cx * cx + cy * cy <= r * r){
                if (y < 0 || y >= c->height || x < 0 || x >= c->width) continue;
                c->pixels[y * c->width + x] ^= 0xffffff;
            }
        }
    }
}

uint8_t
lerpU8(uint8_t a, uint8_t b, float pos)
{
    uint8_t ret;

    int16_t d = b - a;

    if (d == 0) return a;
    if (d > 0) ret = a + (int) (d * pos);
    if (d < 0) ret = a - (int) (-d * pos);

    return ret;
}

uint32_t eLerpColor(uint32_t c1, uint32_t c2, float pos)
{
    uint32_t ret = 0;
    uint8_t r, g, b;
    r = lerpU8((c1 >> 16) & 0xff, (c2 >> (2 * 8)) & 0xff, pos);
    g = lerpU8((c1 >> 8 ) & 0xff, (c2 >> (1 * 8)) & 0xff, pos);
    b = lerpU8( c1        & 0xff, (c2 >> (0 * 8)) & 0xff, pos);

    ret = r << 16 | g << 8 | b;

    return ret;
}

void eInvertPixel(eCanvas *c, int x, int y){
    int pos = y * c->width + x;
    c->pixels[pos] ^= 0xffffff;
}

void eFillPolygon(eCanvas *c, V2 *points, int point_amount, uint32_t color) {
    if (point_amount < 3) {
        return;
    }

    int min_y = points[0].y;
    int max_y = points[0].y;
    for (int i = 1; i < point_amount; i++) {
        if (points[i].y < min_y) {
            min_y = points[i].y;
        }
        if (points[i].y > max_y) {
            max_y = points[i].y;
        }
    }

    for (int y = min_y; y <= max_y; y++) {
        int intersections[point_amount];
        int num_intersections = 0;

        for (int i = 0; i < point_amount; i++) {
            int j = (i + 1) % point_amount; // Next point
            if ((points[i].y < y && points[j].y >= y) || (points[j].y < y && points[i].y >= y)) {
                intersections[num_intersections++] = points[i].x + (double)(y - points[i].y) / (points[j].y - points[i].y) * (points[j].x - points[i].x);
            }
        }

        for (int i = 0; i < num_intersections - 1; i++) {
            for (int j = 0; j < num_intersections - i - 1; j++) {
                if (intersections[j] > intersections[j + 1]) {
                    int temp = intersections[j];
                    intersections[j] = intersections[j + 1];
                    intersections[j + 1] = temp;
                }
            }
        }

        for (int i = 0; i < num_intersections; i += 2) {
            eDrawLine(c, intersections[i], y, intersections[i + 1], y, color);
        }
    }
}

