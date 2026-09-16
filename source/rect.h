#ifndef RECT_H
#define RECT_H

#include <stdbool.h>

typedef struct {
    int x, y;
    int w, h;
} Rect;

Rect getWorldHitbox(int x, int y, Rect* hitbox);
bool rectOverlap(Rect a, Rect b);

#endif