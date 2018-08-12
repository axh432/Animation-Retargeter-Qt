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
    initializeOpenGLFunctions();
    engine.reset(new Engine());
}

void GLWidget::updateWorld(qint64 delta){
    makeCurrent();
    engine->updateWorld(delta);
    doneCurrent();
}

void GLWidget::handleKeyEvent(QKeyEvent *e){
    engine->handleKeyEvent(e);
}

void GLWidget::paintGL()
{
    engine->render();
}

void GLWidget::resizeGL(int w, int h)
{
    engine->resize(w, h);
}
