#ifndef AUDIODATA_H
#define AUDIODATA_H

class AudioData{

public:
    void setRIFF(char riffValue[4]);
    char * getRIFF();

    void setFileSize(unsigned int fileSizeValue);
    unsigned int getFileSize();

    void setFileType(char fileTypeValue[4]);
    char * getFileType();

    void setFormat(char formatValue[4]);
    char * getFormat();

    void setFormatInfoSize(unsigned int formatInfoSizeValue);
    unsigned int getFormatInfoSize();

    void setFormatInfo(char * formatInfoPtr);
    char * getFormatInfo();

    unsigned short getChannels();
    unsigned int getSamplesPerSecond();

    void setDataChunk(char dataChunkValue[4]);
    char * getDataChunk();

    void setDataSize(int dataSizeValue);
    unsigned int getDataSize();

    void setNumberOfSamples(unsigned int number);
    unsigned int getNumberOfSamples();

    void setChannelData(short ** dataPtr);
    short ** getChannelData();

    void freeMemory();

private:
    char riff[5]; //Always set to "RIFF"
    unsigned int fileSize;
    char fileType[5]; //Should always be type "WAVE" for a .wav file
    char format[5]; //Always set to "fmt "
    unsigned int formatInfoSize;
    char * formatInfo;
    unsigned short channels;
    unsigned int samplesPerSecond;
    char dataChunk[5];
    unsigned int dataSize;
    unsigned int numberOfSamples;
    short ** channelData;

};

#endif
