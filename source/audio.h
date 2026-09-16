#ifndef AUDIO_H_
#define AUDIO_H_
#include <maxmod9.h>

typedef struct {
    mm_word* tracks;
    int tracksLen;
    mm_word* sfxs;
    int sfxsLen;
} AudioAssets;

void initAudio(void);
void playSfx(mm_word sfx);
void playMusic(mm_word track, bool loop);
void stopMusic();
void loadAudio(AudioAssets* audioAssets);

#endif