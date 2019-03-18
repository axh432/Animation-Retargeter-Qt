#include "mainwindow.h"
#include "glwidget.h"
#include "loaddialog.h"
#include "retargetdialog.h"
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
    QMenu * viewMenu = menuBar->addMenu(tr("&View"));

    //File menu
    QAction * newScene = new QAction(tr("&New Scene"), this);
    QAction * retargetAnim =    new QAction(tr("&Retarget Animation"), this);
    QAction * exitAct =         new QAction(tr("&Exit"), this);

    connect(newScene, SIGNAL(triggered()), this, SLOT(onNewScene()));
    connect(retargetAnim,   SIGNAL(triggered()), this, SLOT(retargetAnim()));
    connect(exitAct,        SIGNAL(triggered()), this, SLOT(endSession()));

    fileMenu->addAction(newScene);
    fileMenu->addAction(retargetAnim);
    fileMenu->addAction(exitAct);

    //Animation menu
    QAction * playSource = new QAction(tr("&Play Source"), this);
    QAction * playDest = new QAction(tr("&Play Destination"), this);
    QAction * bindSource = new QAction(tr("&Bind Pose Source"), this);
    QAction * bindDest = new QAction(tr("&Bind Pose Destination"), this);
    QAction * pauseSource = new QAction(tr("&Pause Source"), this);
    QAction * pauseDest = new QAction(tr("&Pause Destination"), this);

    QAction * hideSource = new QAction(tr("&Hide Source"), this);
    QAction * hideDest = new QAction(tr("&Hide Destination"), this);
    QAction * showSource = new QAction(tr("&Show Source"), this);
    QAction * showDest = new QAction(tr("&Show Destination"), this);

    connect(playSource, SIGNAL(triggered()), this, SLOT(receiveAnimCommandPlaySource()));
    connect(playDest, SIGNAL(triggered()), this, SLOT(receiveAnimCommandPlayDest()));
    connect(bindSource, SIGNAL(triggered()), this, SLOT(receiveAnimCommandBindSource()));
    connect(bindDest, SIGNAL(triggered()), this, SLOT(receiveAnimCommandBindDest()));;
    connect(pauseSource, SIGNAL(triggered()), this, SLOT(receiveAnimCommandPauseSource()));
    connect(pauseDest, SIGNAL(triggered()), this, SLOT(receiveAnimCommandPauseDest()));

    connect(hideSource, SIGNAL(triggered()), this, SLOT(receiveViewCommandHideSource()));
    connect(hideDest, SIGNAL(triggered()), this, SLOT(receiveViewCommandHideDest()));
    connect(hideSource, SIGNAL(triggered()), this, SLOT(receiveViewCommandShowSource()));
    connect(hideDest, SIGNAL(triggered()), this, SLOT(receiveViewCommandShowDest()));

    animMenu->addAction(playSource);
    animMenu->addAction(pauseSource);
    animMenu->addAction(bindSource);
    animMenu->addSeparator();
    animMenu->addAction(playDest);
    animMenu->addAction(pauseDest);
    animMenu->addAction(bindDest);

    viewMenu->addAction(showSource);
    viewMenu->addAction(hideSource);
    viewMenu->addSeparator();
    viewMenu->addAction(showDest);
    viewMenu->addAction(hideDest);

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

void MainWindow::receiveAnimCommandPlaySource(){
    glWidget->changeSourceAnimState(AnimState::PLAY);
}

void MainWindow::receiveAnimCommandPlayDest(){
    glWidget->changeDestinationAnimState(AnimState::PLAY);
}

void MainWindow::receiveAnimCommandPauseSource(){
    glWidget->changeSourceAnimState(AnimState::PAUSE);
}

void MainWindow::receiveAnimCommandPauseDest(){
    glWidget->changeDestinationAnimState(AnimState::PAUSE);
}

void MainWindow::receiveAnimCommandBindSource(){
    glWidget->changeSourceAnimState(AnimState::BINDPOSE);
}

void MainWindow::receiveAnimCommandBindDest(){
    glWidget->changeDestinationAnimState(AnimState::BINDPOSE);
}

void MainWindow::receiveViewCommandHideSource(){
    glWidget->changeSourceVisualState(VisualState::HIDDEN);
}

void MainWindow::receiveViewCommandHideDest(){
    glWidget->changeDestinationVisualState(VisualState::HIDDEN);
}

void MainWindow::receiveViewCommandShowSource(){
    glWidget->changeSourceVisualState(VisualState::VISIBLE);
}

void MainWindow::receiveViewCommandShowDest(){
    glWidget->changeDestinationVisualState(VisualState::VISIBLE);
}

void MainWindow::onNewScene(){
    LoadDialog *dialog = new LoadDialog(this, glWidget.get());
    dialog->exec();
    qDebug() << "Receive Load Command";
}

void MainWindow::retargetAnim(){

    QPair<Skeleton*, Skeleton*> skeletons = glWidget->getSkeletonsForRetargeting();

    if(skeletons.first && skeletons.second){
        RetargetDialog *dialog = new RetargetDialog(this, skeletons, glWidget.get());
        dialog->exec();
    }else{
        QMessageBox::warning(this, tr("Nothing to Retarget: "), "You have not loaded a scene and so cannot retarget anything.", QMessageBox::Ok);
    }

    qDebug() << "Receive Retarget Command";
}

void MainWindow::run(){

    //renderloop
    qDebug("Entering RenderLoop");

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


