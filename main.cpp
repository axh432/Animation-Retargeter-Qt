#include <iostream>
#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>
#include <QTime>
#include "ui/mainwindow.h"
#include "ui/glwidget.h"
#include <memory>

using std::unique_ptr;

void setUpWindowArea(MainWindow* mainWindow){
    mainWindow->resize(mainWindow->sizeHint());
    int desktopArea = QApplication::desktop()->width() *  QApplication::desktop()->height();
    int widgetArea = mainWindow->width() * mainWindow->height();
    if (((float)widgetArea / (float)desktopArea) < 0.80f)
        mainWindow->show();
    else
        mainWindow->showMaximized();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //set format
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(fmt);

    //declare variables
    MainWindow mainWindow(&app);

    //Set Widget size realtive to screen size
    setUpWindowArea(&mainWindow);
    
    mainWindow.run();

}


