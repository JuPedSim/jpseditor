//main.cpp


#include "mainWindow.h"
#include "GraphicView.h"
#include <QApplication>

int main(int argc, char* argv[]){

    QApplication app(argc,argv);
    MWindow mywindow;
    mywindow.show();
    return app.exec();
}



