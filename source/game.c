#include <nds.h>
#include <maxmod9.h>

#include "definitions.h"
#include "helpers.h"

#include "game.h"

#include "graphics.h"
#include "bg.h"
#include "ground.h"
#include "charIdle.h"
#include "charRun.h"
#include "slime.h"

#include "audio.h"

#include "soundbank.h"
#include "soundbank_bin.h"

game_state_t gameState = BEGIN;
char_state_t char_state = RUN;

PrintConsole topConsole;

Sprite slimes[MAX_SLIMES];
bool slimesActive[MAX_SLIMES];

u16 *charIdleFrames[IDLE_FRAMES];
Animation charIdle;

u16 *charRunFrames[RUN_FRAMES];
Animation charRun;

u16 *slimeFrames[SLIME_FRAMES];
Animation slimeAnim;

Sprite player;
Rect playerRect = {8, 8, 14, 19};

Rect slimesRect = {7, 26, 16, 7};

const BgAsset skyAsset = {
    bgTiles,
    bgTilesLen,
    bgMap,
    bgMapLen,
    bgPal,
    bgPalLen,
    true,
    BgSize_T_256x256
};

const BgAsset groundAsset = {
    groundTiles,
    groundTilesLen,
    groundMap,
    groundMapLen,
    groundPal,
    groundPalLen,
    true,
    BgSize_T_256x256
};

mm_word tracks[] = {MUSIC_TRACK};
mm_word sfxs[] = {SFX_HURT, SFX_JUMP};
AudioAssets audioAssets = {
    tracks,
    1,
    sfxs,
    2
};

int ground;
int sky;

int sky_x = 0 << 8;
int ground_x = 0;
int velocity_y = 0 << 8;
bool grounded = true;
int score = 0;

int delayCount = 0;
int nextDelay = 60;

void printTopText(int score) {
    consoleSetColor(&topConsole, CONSOLE_WHITE);
    consoleSelect(&topConsole);
    consoleClear();
    consoleSetColor(&topConsole, 13);
    consoleSetCursor(&topConsole, 12, 3);
    printf("Score: %d", score);
}

int findFreeSlime(void) {
    for (int i = 0; i < MAX_SLIMES; i++) {
        if (!slimesActive[i])
            return i;
    }
    return -1;
}

void spawnSlime(int x, int y) {
    int i = findFreeSlime();
    if (i == -1) return;

    slimesActive[i] = true;
    slimes[i].x = x;
    slimes[i].y = y;
    slimes[i].frame = 0;
}

bool checkCollision(Sprite* a, Sprite* b, Rect* ra, Rect* rb) {
    Rect world_rect_a = getWorldHitbox(a->x, a-> y, ra);
    Rect world_rect_b = getWorldHitbox(b->x, b-> y, rb);
    return rectOverlap(world_rect_a, world_rect_b);
}

void LoadGame(void){

    loadAudio(&audioAssets);

    ground = createBg(1, &groundAsset);
    sky = createBg(2, &skyAsset);    
    int consoleBg = createConsoleBg(0, &topConsole, true);
    setGlobalBgPal(groundPal, groundPalLen);

    bgShow(ground);
    bgShow(sky);

    createAnimation(&charIdle,
        charIdleFrames,
        charIdleTiles,
        IDLE_FRAMES,
        SpriteSize_32x32, 
        SpriteColorFormat_16Color,
        32, 32
    );

    createAnimation(&charRun,
        charRunFrames,
        charRunTiles,
        RUN_FRAMES,
        SpriteSize_32x32, 
        SpriteColorFormat_16Color,
        32, 32
    );

    createAnimation(&slimeAnim,
        slimeFrames,
        slimeTiles,
        SLIME_FRAMES,
        SpriteSize_32x32, 
        SpriteColorFormat_16Color,
        32,
        32
    );

    int charPalIdx = createSpritePal16(charIdlePal, charIdlePalLen);
    int slimePalIdx = createSpritePal16(slimePal, slimePalLen);

    initSprite(&player,
        0, // id
        50, 118, // x y
        0, // priority
        charPalIdx,
        &charIdle,
        3 // Frame delay
    );

    for (int i = 0; i < MAX_SLIMES; i++){
        int currentId = i + 1; // account for player id
        slimesActive[i] = false;
        initSprite(&slimes[i],
            currentId, // id
            280, 118, // x y
            0, // priority
            slimePalIdx,
            &slimeAnim,
            12 // Frame delay
        );
    }
}
void UnloadGame(void){
    soundDisable();

    freeAnimation(&charIdle);
    freeAnimation(&charRun);
    freeAnimation(&slimeAnim);
}

void UpdateSlimes(void);
void UpdatePlayer(void);

void GameStart(void){
    sky_x = 0 << 8;
    ground_x = 0;
    velocity_y = 0 << 8;
    grounded = true;
    score = 0;
    player.x = 50;
    player.y = 118;

    playMusic(MUSIC_TRACK, true);
}

void UpdateGame(void){
    u16 keys_held = keysHeld();
    u16 keys_down = keysDown();

    switch (gameState)
    {
    case BEGIN:
        score = 0;
        printTopText(score);
        updateAnimation(&player);
        if (keys_down & (KEY_A | KEY_TOUCH)) {
            mmEffect(SFX_JUMP);
            velocity_y = -1450;
            grounded = false;
            gameState = PLAYING;
            setAnimation(&player, &charRun);
        }
        break;
    case PLAYING:
        updateAnimation(&player);
        for (int i = 0; i < MAX_SLIMES; i++) {
            if (!slimesActive[i]) continue;
            slimes[i].x -= 2;
            if (slimes[i].x < -30) {
                slimesActive[i] = false;
                score++;
                printTopText(score);
            }
            
            if (checkCollision(&player, &slimes[i], &playerRect, &slimesRect)){
                playSfx(SFX_HURT);
                stopMusic();
                gameState = END;
                for (int i = 0; i < MAX_SLIMES; i++){ 
                    slimes[i].x = 280;
                    slimesActive[i] = false;
                }
            }
        }
        
        delayCount++;
        if (delayCount > nextDelay){
            delayCount = 0;
            nextDelay = randomRange(28, 50);
            spawnSlime(280, 118);
        }
        
        velocity_y += 110;
        player.y += velocity_y >> 8;
        
        if (player.y >= 118) {
            player.y = 118;
            grounded = true;
        }

        if ((keys_down & (KEY_A | KEY_TOUCH)) && grounded) {
            velocity_y = -1450;
            grounded = false;
            playSfx(SFX_JUMP);
        }
        
        break;
    case END:
        if (keys_down & (KEY_A | KEY_TOUCH)) {
            gameState = BEGIN;
            setAnimation(&player, &charIdle);
            playMusic(MUSIC_TRACK, true);
        }
        break;
    default:
        break;
    }
}
void DrawGame(void){
    graphicsBeginFrame();

        switch (gameState)
        {
        case BEGIN:
            ground_x = 0;
            sky_x = 0;
            bgSetScroll(ground, 0, 0);
            bgSetScroll(sky, 0, 0);
            drawSprite(&player);
            for (int i = 0; i < MAX_SLIMES; i++) {
                drawSprite(&slimes[i]);
            }
            break;
        case PLAYING:
            sky_x += 50;
            ground_x += 2;
            bgSetScroll(ground, ground_x, 0);
            bgSetScroll(sky, sky_x >> 8, 0);
            drawSprite(&player);
            for (int i = 0; i < MAX_SLIMES; i++) {
                if (!slimesActive[i]) continue;
                updateAnimation(&slimes[i]);
                drawSprite(&slimes[i]);
            }
            break;
        case END:
            for (int i = 0; i < MAX_SLIMES; i++) {
                if (!slimesActive[i]) continue;
                drawSprite(&slimes[i]);
            }
            break;
        default:
            break;
        }

    graphicsEndFrame();
}