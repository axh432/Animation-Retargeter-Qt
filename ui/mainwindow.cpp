#include "mainwindow.h"
#include "glwidget.h"
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QApplication>
#include <QKeyEvent>
#include <QThread>
#include <iostream>

MainWindow::MainWindow(QApplication * newApp):
    timer(new QElapsedTimer()),
    glWidget(new GLWidget()),
    app(newApp),
    running(true)
{

    QMenuBar *menuBar = new QMenuBar(this);
    QMenu * fileMenu = menuBar->addMenu(tr("&File"));
    QMenu * animMenu = menuBar->addMenu(tr("&Animation"));

    //File menu
    QAction * openFromMesh = new QAction(tr("&Open From Mesh"), this);
    connect(openFromMesh, SIGNAL(triggered()), this, SLOT(onOpenFromMesh()));

    QAction * openToMesh = new QAction(tr("&Open To Mesh"), this);
    connect(openToMesh, SIGNAL(triggered()), this, SLOT(onOpenToMesh()));

    QAction * openAnim = new QAction(tr("&Open Animation"), this);
    connect(openAnim, SIGNAL(triggered()), this, SLOT(onOpenAnimation()));

    QAction * exitAct = new QAction(tr("&Exit"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(endSession()));

    fileMenu->addAction(openFromMesh);
    fileMenu->addAction(openToMesh);
    fileMenu->addAction(openAnim);
    fileMenu->addAction(exitAct);

    setMenuBar(menuBar);
    setCentralWidget(glWidget.get());

}

MainWindow::~MainWindow(){
    glWidget.reset(nullptr);
    std::cout << "Deleting MainWindow" << std::endl;
}

void MainWindow::endSession(){
    running = false;
}


void MainWindow::onOpenToMesh(){
    /*LoadDialog *dialog = new LoadDialog(this, engine, TO);
    dialog->exec();
    window->resetRetargetWidgets();*/
    
    std::cout << "Open Destination Mesh" << std::endl;
}

void MainWindow::onOpenAnimation(){
    /*LoadDialog * dialog = new LoadDialog(this, engine, ANIM);
    dialog->exec();
    window->resetRetargetWidgets();*/
    
    std::cout << "Open Animation" << std::endl;
}

void MainWindow::onOpenFromMesh(){
    /*LoadDialog *dialog = new LoadDialog(this, engine, FROM);
    dialog->exec();
    window->resetRetargetWidgets();*/
    
    std::cout << "Open Source Mesh" << std::endl;
}

void MainWindow::run(){

    //renderloop
    qDebug("Entering RenderLoop");
    glWidget->createEntities();

   while(running){
        timer->restart();

        app->processEvents();

        glWidget->updateWorld(timer->elapsed());

        glWidget->update(); //paint

        //delay if we still have time to spare
        //qint64 delay = MILLISECONDS_PER_FRAME - timer->elapsed();
        //QThread::msleep(delay > 0? delay : 0);
   }

}

void MainWindow::closeEvent(QCloseEvent *event){
    endSession();
}

QSize MainWindow::sizeHint() const{
    return QSize(1152, 720);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    glWidget->handleKeyEvent(e);
}


