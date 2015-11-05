#include <iostream>
#include <fstream>

#include "coder.h"

//Decodes the data in a WAV file and saves it into an AudioData class
AudioData * coder::decode(string filename){
    fstream audioFile;
    audioFile.open(filename, ios_base::in | ios_base::binary);

    //File does not exist
    if(audioFile.fail()){
        return NULL;
    }

    AudioData * data = NULL;
    if(audioFile.is_open()){
        data = new AudioData;

        //Read RIFF
        int smallBuffer;
        audioFile.read((char *)&smallBuffer, 4);
        data->setRIFF((char *)&smallBuffer);

        //Read file size
        audioFile.read((char *)&smallBuffer, 4);
        data->setFileSize(smallBuffer);

        //Read file type
        audioFile.read((char *)&smallBuffer, 4);
        data->setFileType((char *)&smallBuffer);

        //Read format
        audioFile.read((char *)&smallBuffer, 4);
        data->setFormat((char *)&smallBuffer);

        //Read format info size
        unsigned int infoSize = 0;
        audioFile.read((char *)&infoSize, 4);
        data->setFormatInfoSize(infoSize);

        //Read format info
        char * largeBuffer = (char *)malloc(sizeof(char) * infoSize); //remember to free on close
        memset(largeBuffer, 0x00, infoSize);
        audioFile.read(largeBuffer, infoSize);
        data->setFormatInfo(largeBuffer);

        //Read data chunk
        audioFile.read((char *)&smallBuffer, 4);
        data->setDataChunk((char *)&smallBuffer);

        //Read data size
        unsigned int dataSize = 0;
        audioFile.read((char *)&dataSize, 4);
        data->setDataSize(dataSize);

        //Read audio data
        largeBuffer = (char *)malloc(sizeof(char) * dataSize);
        memset(largeBuffer, 0x00, dataSize);
        audioFile.read(largeBuffer, dataSize);

        //Seperate audio data into multiple channels
        short ** channelData = seperateChannels((short *)largeBuffer, data->getChannels(), data->getNumberOfSamples());
        data->setChannelData(channelData);
        free(largeBuffer);

        audioFile.close();
    }

    return data;
}

//Encodes the audio data into a WAV file
void coder::encode(AudioData * data, string filename){
    ofstream audioFile(filename, ios::binary);

    if(audioFile.is_open()){
        audioFile.write(data->getRIFF(), 4);

        int fileSize = data->getFileSize();
        audioFile.write((char *)&fileSize, 4);

        audioFile.write(data->getFileType(), 4);

        audioFile.write(data->getFormat(), 4);

        unsigned int infoSize = data->getFormatInfoSize();
        audioFile.write((char *)&infoSize, 4);

        audioFile.write(data->getFormatInfo(), infoSize);

        audioFile.write(data->getDataChunk(), 4);

        unsigned int dataSize = data->getDataSize();
        audioFile.write((char *)&dataSize, 4);

        short ** channelData = data->getChannelData();
        short * combinedData = combineChannels(channelData, data->getChannels(), data->getNumberOfSamples());
        audioFile.write((char *)combinedData, dataSize);

        audioFile.close();
    }
}

//Seperates a single array of samples into multiple channels based on the number of channels
short ** coder::seperateChannels(short * data, int channels, int numberOfSamples){
    short ** channelData = (short **)malloc(sizeof(short *) * channels);

    //Create memory for each channel
    for(int i = 0; i < channels; i++){
        channelData[i] = (short *)malloc(sizeof(short) * numberOfSamples);
    }

    //Copy each channel's data into the respective array
    for(int i = 0; i < numberOfSamples; i++){
        for(int j = 0; j < channels; j++){
            channelData[j][i] = data[(i * channels) + j];
        }
    }

    return channelData;
}

//Combines multiple channel samples into a single array of samples
short * coder::combineChannels(short ** channelData, int channels, int numberOfSamples){
    short * data = (short *)malloc(sizeof(short) * (channels * numberOfSamples));

    for(int i = 0; i < numberOfSamples; i++){
        for(int j = 0; j < channels; j++){
            data[(i * channels) + j] = channelData[j][i];
        }
    }

    return data;
}
