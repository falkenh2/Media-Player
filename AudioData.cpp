#include "AudioData.h"
#include <stdlib.h>

void AudioData::setRIFF(char riffValue[4]){
    *(int *)riff = *(int *)riffValue;
    riff[4] = '\0';
}

char * AudioData::getRIFF(){
    return riff;
}

void AudioData::setFileSize(unsigned int fileSizeValue){
    fileSize = fileSizeValue;
}

unsigned int AudioData::getFileSize(){
    return fileSize;
}

void AudioData::setFileType(char fileTypeValue[4]){
    *(int *)fileType = *(int *)fileTypeValue;
    fileType[4] = '\0';
}

char * AudioData::getFileType(){
    return fileType;
}

void AudioData::setFormat(char formatValue[4]){
    *(int *)format = *(int *)formatValue;
    format[4] = '\0';
}

char * AudioData::getFormat(){
    return format;
}

void AudioData::setFormatInfoSize(unsigned int formatInfoSizeValue){
    formatInfoSize = formatInfoSizeValue;
}

unsigned int AudioData::getFormatInfoSize(){
    return formatInfoSize;
}

//Saves the format info and gets the channel and samples per second from the information
void AudioData::setFormatInfo(char * formatInfoPtr){
    formatInfo = formatInfoPtr;

    if(formatInfoSize == 16){
        channels = *(short *)&formatInfo[2];
        samplesPerSecond = *(int *)&formatInfo[4];
    }
}

char * AudioData::getFormatInfo(){
    return formatInfo;
}

unsigned short AudioData::getChannels(){
    return channels;
}

unsigned int AudioData::getSamplesPerSecond(){
    return samplesPerSecond;
}

void AudioData::setDataChunk(char dataChunkValue[4]){
    *(int *)dataChunk = *(int *)dataChunkValue;
    dataChunk[4] = '\0';
}

char * AudioData::getDataChunk(){
    return dataChunk;
}

void AudioData::setDataSize(int dataSizeValue){
    dataSize = dataSizeValue;

    numberOfSamples = dataSize / (2 * channels);
}

unsigned int AudioData::getDataSize(){
    return dataSize;
}

void AudioData::setNumberOfSamples(unsigned int number){
    numberOfSamples = number;
}

unsigned int AudioData::getNumberOfSamples(){
    return numberOfSamples;
}

void AudioData::setChannelData(short ** dataPtr){
    channelData = dataPtr;
}

short ** AudioData::getChannelData(){
    return channelData;
}

void AudioData::freeMemory(){
    free(formatInfo);
    for(int i = 0; i < channels; i++){
        free(channelData[i]);
    }
    free(channelData);
}
