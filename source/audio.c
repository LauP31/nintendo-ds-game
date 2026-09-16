#include <maxmod9.h>
#include "audio.h"

#include "soundbank.h"
#include "soundbank_bin.h"

void initAudio(void){
    mmInitDefaultMem((mm_addr)soundbank_bin);
}

void playSfx(mm_word sfx){
    mmEffect(sfx);
}

void playMusic(mm_word track, bool loop) {
    mm_pmode mode = loop ? MM_PLAY_LOOP : MM_PLAY_ONCE;
    mmStart(MOD_LITTLE_LEAPS, mode);
}

void stopMusic(){
    mmStop();
}

void loadAudio(AudioAssets* audioAssets){
    for (int i = 0; i < audioAssets->tracksLen; i++){
        mmLoad(audioAssets->tracks[i]);
    }

    for (int i = 0; i < audioAssets->sfxsLen; i++){
        mmLoad(audioAssets->sfxs[i]);
    }
}