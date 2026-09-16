#include <stdbool.h>
#include "rect.h"


Rect getWorldHitbox(int x, int y, Rect* hitbox) {
    Rect r;
    r.x = x + hitbox->x;
    r.y = y + hitbox->y;
    r.w = hitbox->w;
    r.h = hitbox->h;
    return r;
}

bool rectOverlap(Rect a, Rect b) {
    return (
        a.x < b.x + b.w &&
        a.x + a.w > b.x &&
        a.y < b.y + b.h &&
        a.y + a.h > b.y
    );
}