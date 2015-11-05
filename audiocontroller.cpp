#include "audiocontroller.h"
#include <string>
#include <QWidget>
#include <QPainter>
#include <QLayout>

//Initialize the variables needed to visualize the wave, and show a blank wave
AudioController::AudioController(QWidget * parent) : QWidget(parent){
    data = NULL;
    player = NULL;
    channels = 1;
    sampletolineRatio = 1000;
    rectangleHeight = 200;
    rectangleSeperator = 20;
    zoomRatio = 1;
    currentFilename = "NULL";

    waveVisuals = new ChannelWave *[1];
    waveScrollAreas = new QScrollArea *[1];
    waveVisuals[0] = new ChannelWave(this);
    waveScrollAreas[0] = new QScrollArea(this);

    waveScrollAreas[0]->move(0, 0);
    waveScrollAreas[0]->resize(size().width() - 10, rectangleHeight);
    waveScrollAreas[0]->setWidget(waveVisuals[0]);
    waveScrollAreas[0]->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    waveScrollAreas[0]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerCallBack()));
    timer->start(500);
}

//Load a file, clearing the current one in memory
void AudioController::loadFile(std::string filename){
    if(data != NULL){
        deleteCurrentFile();
    }

    deleteWidgets();

    data = coder::decode(filename);

    if(data == NULL){
        return;
    }

    channels = data->getChannels();
    currentFilename = filename;
    zoomRatio = 1;
    volume = 100;
    speed = 50;
    waveVisuals = new ChannelWave *[channels];
    waveScrollAreas = new QScrollArea *[channels];

    for(int i = 0; i < channels; i++){
        waveVisuals[i] = new ChannelWave(this);
        waveScrollAreas[i] = new QScrollArea(this);

        waveScrollAreas[i]->move(0, i * (rectangleHeight + rectangleSeperator));
        waveScrollAreas[i]->resize(size().width() - 10, rectangleHeight);
        waveScrollAreas[i]->setWidget(waveVisuals[i]);
        waveScrollAreas[i]->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        waveScrollAreas[i]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        waveVisuals[i]->init(data->getChannelData()[i], data->getNumberOfSamples(), sampletolineRatio);
        waveVisuals[i]->resize(size().width() - 2, rectangleHeight);

        waveVisuals[i]->show();
        waveScrollAreas[i]->show();
    }
}

//Saves a file as the current filename
void AudioController::saveFile(){
    if(data == NULL){
        return;
    }

    coder::encode(data, currentFilename);
}

//Saves a file as the given path/filename
void AudioController::saveFileAs(std::string filename){
    if(data == NULL){
        return;
    }

    coder::encode(data, filename);
    currentFilename = filename;
}

//Starts playing if the file is not paused, otherwise continues playing
int AudioController::playFile(){
    if(data != NULL){
        if(player == NULL){
            player = new AudioPlayer(data);
        }

        player->setSpeed(speed);
        player->setVolume(volume);
        player->playAudio();

        return WORKING;
    }

    return 0;
}

//Pauses the file
void AudioController::pauseFile(){
    if(player != NULL){
        player->pauseAudio();
    }
}

//Stops the file
void AudioController::stopFile(){
    if(player != NULL){
        player->stopAudio();
        player = NULL;

        for(int i = 0; i < channels; i++){
            waveVisuals[i]->updateIndex(0.0);
            waveVisuals[i]->update();
        }
    }
}

//Zooms in on the wave by increasing the width of the wave box
void AudioController::zoomIn(){
    zoomRatio *= 2;
}

//Zooms out on the wave by shortening the width of the wave box
void AudioController::zoomOut(){
    if(zoomRatio != 1){
        zoomRatio = zoomRatio / 2;
    }
}

//Updates the volume in the audio player
void AudioController::changeVolume(int volumeValue){
    volume = volumeValue;
    if(player != NULL){
        player->setVolume(volume);
    }
}

//Updates the speed in the audio player
void AudioController::changeSpeed(int speedValue){
    speed = speedValue;
    if(player != NULL){
        player->setSpeed(speed);
    }
}

//Halves the length of the song by decreasing the number of samples
void AudioController::halveLength(){
    changeLength(data->getNumberOfSamples()/2);
}

//Doubles the length of the song by increasing the number of samples
void AudioController::doubleLength(){
    changeLength(data->getNumberOfSamples()*2);
}

//Resizes all inner widgets when this widget is resized
void AudioController::resizeEvent(QResizeEvent *e){
    Q_UNUSED(e);

    for(int i = 0; i < channels; i++){
        waveScrollAreas[i]->resize(size().width(), rectangleHeight);
        waveVisuals[i]->resize(size().width() * zoomRatio - 2, rectangleHeight);
    }
}

//paints the wave of each audio channel using the sample's value as an offset
void AudioController::paintEvent(QPaintEvent *e){
    Q_UNUSED(e);

    resize(size().width(), channels * (rectangleHeight + 20));

    for(int i = 0; i < channels; i++){
        waveScrollAreas[i]->resize(size().width(), rectangleHeight);
        waveVisuals[i]->resize(size().width() * zoomRatio - 2, rectangleHeight);
        waveVisuals[i]->update();
    }
}

//Clears the current file from memory
void AudioController::deleteCurrentFile(){
    data->freeMemory();
}

//Deletes previous inner widgets
void AudioController::deleteWidgets(){
    for(int i = 0; i < channels; i++){
        delete waveVisuals[i];
        delete waveScrollAreas[i];
    }

    delete[] waveScrollAreas;
    delete[] waveVisuals;
}

//Updates the song index periodically
void AudioController::timerCallBack(){
    float songIndex = 0.0;
    if(player != NULL){
        songIndex = player->getIndex();
    }

    for(int i = 0; i < channels; i++){
        waveScrollAreas[i]->resize(size().width(), rectangleHeight);
        waveVisuals[i]->resize(size().width() * zoomRatio - 2, rectangleHeight);
        waveVisuals[i]->updateIndex(songIndex);
        waveVisuals[i]->update();
    }
}

//Changes the length of the song
void AudioController::changeLength(unsigned int newSampleNumber){
    unsigned int currentSampleNumber = data->getNumberOfSamples();
    float sampleRatio = (float)currentSampleNumber / newSampleNumber;

    short ** channelData = data->getChannelData();

    short ** newChannelData = (short **)malloc(sizeof(short *) * channels);
    for(int i = 0; i < channels; i++){
        newChannelData[i] = (short *)malloc(sizeof(short) * newSampleNumber);
    }

    for(unsigned int i = 0; i < newSampleNumber; i++){
        for(int j = 0; j < channels; j++){
            newChannelData[j][i] = channelData[j][(unsigned int)(i * sampleRatio)];
        }
    }

    for(int i = 0; i < channels; i++){
        channelData[i] = newChannelData[i];
    }

    free(newChannelData);

    data->setNumberOfSamples(newSampleNumber);
    data->setDataSize(data->getDataSize() / sampleRatio);
}
