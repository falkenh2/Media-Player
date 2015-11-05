#include "gui.h"
#include <QApplication>

int main(int argc, char *argv[]){
    //coder::encode(data, "output.wav");

    QApplication a(argc, argv);

    GUI gui;
    gui.show();

    return a.exec();
}
