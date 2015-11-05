#ifndef CHANNELWAVE_H
#define CHANNELWAVE_H

#include <QWidget>
#include <QPainter>

class ChannelWave : public QWidget{
public:
    ChannelWave(QWidget * parent = 0);
    void init(short * data, int numberSamples, int lineToSamples);

    void updateIndex(float index);

private:
    short * sampleData;
    int numOfSamples;
    int sampleToLineRatio;
    float songIndex;

    void paintEvent(QPaintEvent * e);
};

#endif // CHANNELWAVE_H
