// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2024-2026

#include <nds.h>
#include "graphics.h"
#include "audio.h"
#include "game.h"

PrintConsole bottomConsole;

void printBottomText(void) {
    consoleSelect(&bottomConsole);
    printf("\n");
    printf("A or Touch: Jump\n");
    printf("Select: Swap screens\n");
    printf("Start: Exit\n");
    printf("Jump to start!\n");
}

int main(int argc, char *argv[])
{

    srand(timerElapsed(0));
    graphicsInit();
    initAudio();
    LoadGame();
    

    consoleInit(&bottomConsole, 0, BgType_Text4bpp, BgSize_T_256x256, 30, 31, false, true);
    setBackdropColorSub(RGB15(22, 15, 10));
    printBottomText();

    GameStart();

    while (true)
    {
        scanKeys();

        u32 keys_held = keysHeld();
        u32 keys_down = keysDown();


        if (keys_held & KEY_START)
            break;
        
        // if (keys_down & KEY_SELECT)
        //     lcdSwap();

        UpdateGame();
        DrawGame();
    }

    UnloadGame();

    return 0;
}
