#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QScrollArea>
#include <QSlider>
#include "audiocontroller.h"

class GUI : public QMainWindow
{
    Q_OBJECT

public:
    GUI();
    void resizeEvent(QResizeEvent * e);

public slots:
    void openFile();
    void saveFile();
    void saveFileAs();
    void start();
    void stop();
    void pause();
    void zoomIn();
    void zoomOut();
    void volumeChanged();
    void speedChanged();
    void halveLength();
    void doubleLength();

private:
    AudioController * controller;
    QScrollArea * scrollArea;
    QSlider * sliderVolume;
    QSlider * sliderSpeed;

    void initGUI();
    void initMenu();
    void initToolbar();
};

#endif // GUI_H
