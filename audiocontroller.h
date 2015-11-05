#ifndef AUDIOCONTROLLER_H
#define AUDIOCONTROLLER_H
#include <string>
#include <QWidget>
#include <QScrollArea>
#include <QTimer>
#include "coder.h"
#include "AudioData.h"
#include "audioplayer.h"
#include "channelwave.h"

class AudioController : public QWidget{
    Q_OBJECT

public:
    AudioController(QWidget * parent = 0);
    void loadFile(std::string filename);
    void saveFile();
    void saveFileAs(std::string filename);

    int playFile();
    void pauseFile();
    void stopFile();
    void zoomIn();
    void zoomOut();
    void changeVolume(int volumeValue);
    void changeSpeed(int speedValue);
    void halveLength();
    void doubleLength();

    void resizeEvent(QResizeEvent * e);

public slots:
    void timerCallBack();

private:
    int channels;
    int sampletolineRatio;
    int rectangleHeight;
    int rectangleSeperator;
    int zoomRatio;
    int volume;
    int speed;
    std::string currentFilename;
    AudioData * data;
    AudioPlayer * player;
    ChannelWave ** waveVisuals;
    QScrollArea ** waveScrollAreas;
    QTimer * timer;

    void paintEvent(QPaintEvent * e);
    void deleteCurrentFile();
    void deleteWidgets();
    void changeLength(unsigned int newSampleNumber);
};

#endif // AUDIOCONTROLLER_H
