#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QApplication>
#include <math.h>
#include <QTime>
#include <QThread>
#include <QKeyEvent>
#include <iostream>
#include "engine/engine.h"

GLWidget::GLWidget(){    
    //initializeGL();
}

GLWidget::~GLWidget(){
    makeCurrent();
        engine.reset();
    doneCurrent();
    std::cout << "Deleting OpenGLWidget" << std::endl;
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void GLWidget::initializeGL()
{
    qDebug() << "InitializeGL() called";
    initializeOpenGLFunctions();
    engine.reset(new Engine());
}

void GLWidget::createEntities(){
    makeCurrent();
    engine->createEntities();
    doneCurrent();
}

void GLWidget::updateWorld(qint64 delta){
    makeCurrent();
    qDebug() << "updateWorld() called";
    engine->updateWorld(delta);
    doneCurrent();
}

void GLWidget::handleKeyEvent(QKeyEvent *e){
    engine->handleKeyEvent(e);
}

void GLWidget::paintGL()
{
    qDebug() << "paintGL() called";
    engine->render();
}

void GLWidget::resizeGL(int w, int h)
{
    engine->resize(w, h);
}
