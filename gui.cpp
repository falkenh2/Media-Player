#include "gui.h"
#include "ui_gui.h"
#include <string>
#include <QMenu>
#include <QScrollArea>
#include <QFileDialog>

using namespace std;

GUI::GUI(){
    initGUI();
}

//Initialize the gui's different components
void GUI::initGUI(){
    resize(500, 500);

    initMenu();
    initToolbar();

    controller = new AudioController(this);

    scrollArea = new QScrollArea(this);
    scrollArea->move(5, 60);
    scrollArea->resize(size().width() - 10, size().height() - 65);
    scrollArea->setWidget(controller);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    controller->resize(scrollArea->size());
}

//Initialize the menu bar of the window
void GUI::initMenu(){
    QMenu * menu, * edit;
    menu = menuBar()->addMenu("&File");
    edit = menuBar()->addMenu("&Edit");

    QAction * openAction = new QAction("&Open", this);
    QAction * saveAction = new QAction("&Save", this);
    QAction * saveAsAction = new QAction("&Save As", this);
    QAction * quitAction = new QAction("&Close", this);

    QAction * halveLengthAction = new QAction("&Halve Length", this);
    QAction * doubleLengthAction = new QAction("&Double Length", this);

    menu->addAction(openAction);
    menu->addSeparator();
    menu->addAction(saveAction);
    menu->addAction(saveAsAction);
    menu->addSeparator();
    menu->addAction(quitAction);

    edit->addAction(halveLengthAction);
    edit->addAction(doubleLengthAction);

    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveFileAs()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    connect(halveLengthAction, SIGNAL(triggered()), this, SLOT(halveLength()));
    connect(doubleLengthAction, SIGNAL(triggered()), this, SLOT(doubleLength()));
}

//Initialize the toolbar at the top of the window
void GUI::initToolbar(){
    QPixmap openPicture("Images/open.png");
    QPixmap savePicture("Images/save.png");
    QPixmap playPicture("Images/play.png");
    QPixmap pausePicture("Images/pause.png");
    QPixmap stopPicture("Images/stop.png");
    QPixmap zoomInPicture("Images/zoom-in.png");
    QPixmap zoomOutPicture("Images/zoom-out.png");

    QToolBar * toolbar = addToolBar("Toolbar");
    QAction * openAction = toolbar->addAction(QIcon(openPicture), "Open");
    QAction * saveAction = toolbar->addAction(QIcon(savePicture), "Save");
    toolbar->addSeparator();
    QAction * playAction = toolbar->addAction(QIcon(playPicture), "Play");
    QAction * pauseAction = toolbar->addAction(QIcon(pausePicture), "Pause");
    QAction * stopAction = toolbar->addAction(QIcon(stopPicture), "Stop");
    toolbar->addSeparator();
    QAction * zoomInAction = toolbar->addAction(QIcon(zoomInPicture), "Zoom-In");
    QAction * zoomOutAction = toolbar->addAction(QIcon(zoomOutPicture), "Zoom-Out");

    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(playAction, SIGNAL(triggered()), this, SLOT(start()));
    connect(pauseAction, SIGNAL(triggered()), this, SLOT(pause()));
    connect(stopAction, SIGNAL(triggered()), this, SLOT(stop()));
    connect(zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));

    toolbar->addSeparator();
    QPixmap volumePicture("Images/volume.png");
    QAction * volumeIcon = toolbar->addAction(QIcon(volumePicture), "Volume");
    volumeIcon->setEnabled(false);
    sliderVolume = new QSlider(Qt::Horizontal, this);
    sliderVolume->setTickPosition(QSlider::TicksBelow);
    sliderVolume->setTickInterval(10);
    sliderVolume->setValue(100);
    toolbar->addWidget(sliderVolume);

    toolbar->addSeparator();
    QPixmap speedPicture("Images/speed.png");
    QAction * speedIcon = toolbar->addAction(QIcon(speedPicture), "Speed");
    speedIcon->setEnabled(false);
    sliderSpeed = new QSlider(Qt::Horizontal, this);
    sliderSpeed->setTickPosition(QSlider::TicksBelow);
    sliderSpeed->setTickInterval(10);
    sliderSpeed->setValue(50);
    toolbar->addWidget(sliderSpeed);

    connect(sliderVolume, SIGNAL(valueChanged(int)), this, SLOT(volumeChanged()));
    connect(sliderSpeed, SIGNAL(valueChanged(int)), this, SLOT(speedChanged()));
}

//Changes the size of the components when the window is resized
void GUI::resizeEvent(QResizeEvent * e){
    Q_UNUSED(e);

    scrollArea->resize(size().width() - 10, size().height() - 65);
    controller->resize(scrollArea->size().width(), controller->size().height());
}

//Loads a file on open
void GUI::openFile(){
    QString filenameQString = QFileDialog::getOpenFileName(this, tr("Open File"), "./", tr("Files (*.wav)"));
    std::string filename = filenameQString.toUtf8().constData();

    controller->loadFile(filename);
}

//Saves a file on save
void GUI::saveFile(){
    controller->saveFile();
}

//Saves a file with the given path/name on save as
void GUI::saveFileAs(){
    QString filenameQString = QFileDialog::getSaveFileName(this, tr("Save File As"), "./", tr("Files (*.wav)"));
    std::string filename = filenameQString.toUtf8().constData();

    controller->saveFileAs(filename);
}

//Starts playing a file on play
void GUI::start(){
    controller->playFile();
}

//Stops a file on stop
void GUI::stop(){
    controller->stopFile();
}

//Pauses a file on pause
void GUI::pause(){
    controller->pauseFile();
}

//Zooms in on a file's audio wave on zoom-in
void GUI::zoomIn(){
    controller->zoomIn();
}

//Zooms out on a file's audio wave on zoom-out
void GUI::zoomOut(){
    controller->zoomOut();
}

//Updates the song's playback volume when called
void GUI::volumeChanged(){
    controller->changeVolume(sliderVolume->value());
}

//Updates the song's playback speed when called
void GUI::speedChanged(){
    controller->changeSpeed(sliderSpeed->value());
}

//Halves the song length
void GUI::halveLength(){
    controller->halveLength();
}

//Doubles the song length
void GUI::doubleLength(){
    controller->doubleLength();
}
