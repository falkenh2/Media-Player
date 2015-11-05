#include "channelwave.h"

ChannelWave::ChannelWave(QWidget * parent) : QWidget(parent){
    sampleData = NULL;
}

//Initializes the single channel's wave info
void ChannelWave::init(short * data, int numberSamples, int lineToSamples){
    sampleData = data;
    numOfSamples = numberSamples;
    sampleToLineRatio = lineToSamples;
    songIndex = 0.0;
}

//Updates the index of the song being played
void ChannelWave::updateIndex(float index){
    songIndex = index;
}

//Paints the single channel's wave to the screen
void ChannelWave::paintEvent(QPaintEvent * e){
    Q_UNUSED(e);

    QPainter paint(this);
    paint.setBrush(QBrush("#ffffff"));

    if(sampleData != NULL){
        float pixelsToSamplesRatio = size().width() / (float)numOfSamples;
        float pixelsToRange = size().height() / (65535.0/2.0);

        paint.setPen(QColor("#ffffff"));
        paint.drawRect(0, 0, size().width(), size().height());

        paint.setPen(QColor("#000000"));
        int prevSampleHeight = (((65535/2)*pixelsToRange)/2 + size().height());

        for(int j = sampleToLineRatio; j < numOfSamples; j += sampleToLineRatio){
            int sampleHeight = ((sampleData[j] + 65535/2) * pixelsToRange)/2;
            paint.drawLine((j - sampleToLineRatio) * pixelsToSamplesRatio, prevSampleHeight, j * pixelsToSamplesRatio, sampleHeight);

            prevSampleHeight = sampleHeight;
        }

        paint.setPen(QColor("#ff0000"));
        paint.drawLine(songIndex * size().width(), 0, songIndex * size().width(), size().height());
    }else{
        paint.drawRect(0, 0, size().width(), size().height());
    }
}
