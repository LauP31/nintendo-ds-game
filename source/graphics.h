#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <nds.h>

typedef struct {
    const void* tiles;
    int tilesLen;

    const void* map;
    int mapLen;

    const void* pal;
    int palLen;

    int is8bpp;
    BgSize size;
} BgAsset; 

typedef struct {
    u16** frames;
    int frameCount;
    int width;
    int height;

    SpriteSize size;
    SpriteColorFormat colorFormat;

} Animation;

typedef struct {
    int id;
    int x, y;
    int priority;
    int palIndex;
    int frame;
    int frameDelay;
    int frameDelayCounter;

    const Animation* anim;
} Sprite;

void graphicsInit(void);

int allocTileBlock(void);
int allocMapBlock(void);

int createBg(int layer, const BgAsset* asset);

int createConsoleBg(int layer, PrintConsole* console, bool mainDisplay);

void setGlobalBgPal(const void* pal, int palLen);

int createSpritePal16(const void *pal, int palLen);
// u16* createSpriteGfx(const SpriteAsset* asset);

void initSprite(Sprite* s,
                int id,
                int x, int y,
                int priority,
                int palIndex,
                const Animation* anim,
                int frameDelay);

void drawSprite(Sprite* s);
void copy_sprite_frame(void *dst, int frame, const void *tiles, bool is8bp, int width, int height);

void createAnimation(
    Animation* anim,
    u16** frameBuffer,
    const void* tiles,
    int frameCount,
    SpriteSize size,
    SpriteColorFormat format,
    int width,
    int height);

void freeAnimation(Animation* anim);

void setAnimation(Sprite* s, const Animation* anim);
void updateAnimation(Sprite* s);

void graphicsBeginFrame(void);
void graphicsEndFrame(void);

#endif // GRAPHICS_H