#include "definitions.h"
#include "graphics.h"
#include <nds.h>


static int nextBlock = 0;
// static int nextBgPalIndex = 0;
static int nextSpritePalIndex = 0;

void graphicsInit(void) {
    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
    vramSetBankB(VRAM_B_MAIN_SPRITE);
    vramSetBankC(VRAM_C_SUB_BG);
    oamInit(&oamMain, SpriteMapping_1D_128, false);
}

// For 256 color palettes
void setGlobalBgPal(const void* pal, int palLen){
    memcpy(BG_PALETTE, pal,  palLen);
}

void setGlobalSpritePal(const void* pal, int palLen){
    memcpy(SPRITE_PALETTE, pal,  palLen);
}

int allocTileBlock(void) { return nextBlock++; }
int allocMapBlock(void)  { return nextBlock++; }
// int getNextBgPalBlock(void)  { return nextBgPalIndex++; }
int getNextSpritePalBlock(void)  { return nextSpritePalIndex++; }

int createBg(int layer, const BgAsset* asset) {
    int tileBase = allocTileBlock();
    int mapBase  = allocMapBlock();
    printf("%d", tileBase);
    printf("%d", mapBase);
    int bg = bgInitHidden(
        layer,
        asset->is8bpp ? BgType_Text8bpp : BgType_Text4bpp,
        asset->size,
        tileBase,
        mapBase
    );

    memcpy(bgGetGfxPtr(bg), asset->tiles, asset->tilesLen);
    memcpy(bgGetMapPtr(bg), asset->map, asset->mapLen);
    //setGlobalBgPal(asset->pal, asset->palLen);

    return bg;
}

    PrintConsole uiConsole;
    
int createConsoleBg(int layer, PrintConsole* console, bool mainDisplay) {
    int tileBase = allocTileBlock();
    int mapBase  = allocMapBlock();
    consoleInit(console, 0, BgType_Text4bpp, BgSize_T_256x256, mapBase, tileBase, mainDisplay, true);
    int consoleBg = bgInit(0, BgType_Text4bpp, BgSize_T_256x256, mapBase, tileBase);
    return consoleBg;
}

int createSpritePal16(const void *pal, int palLen) {
    int index = getNextSpritePalBlock();
    memcpy(SPRITE_PALETTE + index * 16, pal, palLen);
    return index;
}

// u16* createSpriteGfx(const SpriteAsset* asset) {
//     u16* dest = oamAllocateGfx(&oamMain, asset->size, asset->colorFormat);
//     memcpy(dest, asset->tiles, asset->tilesLen);
//     return dest;
// }

void initSprite(Sprite* s,
                int id,
                int x, int y,
                int priority,
                int palIndex,
                const Animation* anim,
                int frameDelay)
{
    s->id = id;
    s->x = x;
    s->y = y;
    s->priority = priority;
    s->palIndex = palIndex;

    s->anim = anim;
    s->frame = 0;
    s->frameDelay = frameDelay;
    s->frameDelayCounter = 0;
}

void drawSprite(Sprite* s) {
    int palIdx = (s->anim->colorFormat == SpriteColorFormat_256Color) ? 0 : s->palIndex;
    oamSet(&oamMain,
        s->id,
        s->x, s->y,
        s->priority,
        palIdx,
        s->anim->size,
        s->anim->colorFormat,
        s->anim->frames[s->frame],
        -1,
        false,
        false,
        false, false,
        false);
}

void copy_sprite_frame(
                    void *dst,
                    int frame, 
                    const void *tiles,
                    bool is8bpp,
                    int width,
                    int height) {

    int bpp = is8bpp ? 8 : 4;
    uint32_t frame_size =  (width * height * bpp) / 8 ;
    uint32_t offset = frame_size * frame;
    const uint8_t *base = (const uint8_t *)tiles;

    memcpy(dst, base + offset, frame_size);
}

void createAnimation(
                    Animation* anim,
                    u16** frameBuffer,
                    const void* tiles,
                    int frameCount,
                    SpriteSize size,
                    SpriteColorFormat format,
                    int width,
                    int height) {

    anim->frames = frameBuffer;
    anim->frameCount = frameCount;
    anim->size = size;
    anim->colorFormat = format;

    bool is8bpp = (format == SpriteColorFormat_256Color);

    for (int i = 0; i < frameCount; i++) {
        frameBuffer[i] = oamAllocateGfx(&oamMain, size, format);

        copy_sprite_frame(frameBuffer[i],
                          i,
                          tiles,
                          is8bpp,
                          width,
                          height);
    }
}

void freeAnimation(Animation* anim){
    for (int i = 0; i < anim->frameCount; i++)
        oamFreeGfx(&oamMain, anim->frames);
}

void setAnimation(Sprite* s, const Animation* anim) {
    if (s->anim == anim) return;

    s->anim = anim;
    s->frame = 0;
    s->frameDelayCounter = 0;
}

void updateAnimation(Sprite* s) {
    if (!s->anim || s->anim->frameCount == 0) return;

    s->frameDelayCounter++;

    if (s->frameDelayCounter >= s->frameDelay) {
        s->frameDelayCounter = 0;

        s->frame++;
        if (s->frame >= s->anim->frameCount)
            s->frame = 0;
    }
}

void graphicsBeginFrame(void) {
    swiWaitForVBlank();
}

void graphicsEndFrame(void) {
    bgUpdate();
    oamUpdate(&oamMain);
}