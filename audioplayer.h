#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <windows.h>
#include <process.h>
#include <MMSystem.h>
#include <stdio.h>
#include "AudioData.h"

#pragma comment(lib, "winmm.lib")

#define DEVICE_ERROR 0
#define WORKING 1

typedef struct PlayingData{
    bool * isPlaying;
    AudioData * data;
    HWAVEOUT * deviceHandler;
    unsigned int * sampleIndex;
    int * volume;
    int * speed;
} PlayingData;

class AudioPlayer
{
public:
    AudioPlayer(AudioData * data);

    int playAudio();
    void stopAudio();
    void pauseAudio();
    void setVolume(int volumeValue);
    void setSpeed(int speedValue);

    float getIndex();

private:
    bool isPlaying;
    bool isPaused;
    int volume;
    int speed;
    unsigned int sampleIndex;
    AudioData * audioData;
    HWAVEOUT deviceHandler;
    WAVEFORMATEX wfx;
    PlayingData threadData;

    void cleanUp();
};

void bufferData(void * ptr);

#endif // AUDIOPLAYER_H
