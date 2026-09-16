#ifndef GAME_H
#define GAME_H

#include "graphics.h"
#include "rect.h"

typedef enum
{
    BEGIN,
    PLAYING,
    END
} game_state_t;

typedef enum
{
    IDLE,
    RUN
}
char_state_t;

void LoadGame(void);
void UnloadGame(void);
void GameStart(void);
void UpdateSlimes(void);
void UpdatePlayer(void);
void UpdateGame(void);
void DrawGame(void);
void GameEnd(void);

bool checkCollision(Sprite* a, Sprite* b, Rect* ra, Rect* rb);

#endif // GAME_H