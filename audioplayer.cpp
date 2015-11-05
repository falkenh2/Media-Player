#include "audioplayer.h"

//Initializes all the header information needed to connect to the audio device
AudioPlayer::AudioPlayer(AudioData *data)
{
    isPlaying = false;
    isPaused = false;
    audioData = data;

    wfx.nSamplesPerSec = audioData->getSamplesPerSecond();
    wfx.wBitsPerSample = 16;
    wfx.nChannels = audioData->getChannels();
    wfx.cbSize = 0;
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nBlockAlign = 2 * audioData->getChannels();
    wfx.nAvgBytesPerSec = audioData->getSamplesPerSecond() / audioData->getChannels();
}

//Plays the audio if the player is stopped, continues from the previous index if it was paused.
int AudioPlayer::playAudio(){
    if(isPlaying){
        return WORKING;
    }

    if(waveOutOpen(&deviceHandler, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR){
        return DEVICE_ERROR;
    }

    if(isPaused){
        isPaused = false;
    }else{
        sampleIndex = 0;
    }

    isPlaying = true;
    threadData.data = audioData;
    threadData.isPlaying = &isPlaying;
    threadData.deviceHandler = &deviceHandler;
    threadData.sampleIndex = &sampleIndex;
    threadData.volume = &volume;
    threadData.speed = &speed;
    _beginthread(bufferData, 0, &threadData);

    return WORKING;
}

//Tells the buffer to pause
void AudioPlayer::pauseAudio(){
    isPaused = true;
    isPlaying = false;
}

//Tells the buffer to stop
void AudioPlayer::stopAudio(){
    isPlaying = false;

    cleanUp();
}

//Updates the playback volume
void AudioPlayer::setVolume(int volumeValue){
    volume = volumeValue;
}

//Updates the playback speed
void AudioPlayer::setSpeed(int speedValue){
    speed = speedValue;
    if(speed == 0){
        speed = 1;
    }
}

//Returns the index of the song by percentage
float AudioPlayer::getIndex(){
    float index = sampleIndex / (float)(audioData->getNumberOfSamples());
    return index;
}

//Buffers the data into a single array and sends it block by block to the sound device
void bufferData(void * ptr){
    PlayingData * data = (PlayingData *)ptr;
    AudioData * audioData = data->data;
    bool * isPlaying = data->isPlaying;
    HWAVEOUT * deviceHandler = data->deviceHandler;
    unsigned int sampleIndex = *data->sampleIndex;
    WAVEHDR * header;

    int channels = audioData->getChannels();
    while(*isPlaying && sampleIndex < audioData->getNumberOfSamples()){
        header = (WAVEHDR *)malloc(sizeof(WAVEHDR));
        short * block = new short[4000 * channels];
        int size = 0;
        int blockIndex = 0;
        for(double i = sampleIndex; (unsigned int)(i) < sampleIndex + (unsigned int)(4000 * (float)(*data->speed / 50.0)) && (unsigned int)(i) < audioData->getNumberOfSamples(); i += (float)(*data->speed / 50.0)){
            for(int j = 0; j < channels; j++){
                block[((blockIndex) * channels) + j] = (short)((float)(*data->volume/100.0) * audioData->getChannelData()[j][(unsigned)(i)]);
                size++;
            }
            blockIndex++;
        }

        memset(header, 0x00, sizeof(WAVEHDR));
        header->dwBufferLength = size * 2;
        header->lpData = (LPSTR)((void *)block);

        waveOutPrepareHeader(*deviceHandler, header, sizeof(WAVEHDR));

        waveOutWrite(*deviceHandler, header, sizeof(WAVEHDR));

        sampleIndex += (unsigned int)(4000 * (float)(*data->speed / 50.0));

        *data->sampleIndex = sampleIndex;

        Sleep(4000 / (audioData->getSamplesPerSecond() / 1000));
    }

    *data->sampleIndex = sampleIndex;

    _endthread();
}

//Cleans up everything needing to be deleted
void AudioPlayer::cleanUp(){
    waveOutClose(deviceHandler);
}
