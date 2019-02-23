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

void GLWidget::loadEntities(std::shared_ptr<EntityResourcePaths> sourcePaths, std::shared_ptr<EntityResourcePaths> destinationPaths){
    makeCurrent();
    engine->createEntities(sourcePaths, destinationPaths);
    doneCurrent();
}

void GLWidget::changeSourceAnimState(AnimState newState) {
    makeCurrent();
    engine->changeSourceAnimState(newState);
    doneCurrent();
}
void GLWidget::changeDestinationAnimState(AnimState newState) {
    makeCurrent();
    engine->changeDestinationAnimState(newState);
    doneCurrent();
}

void GLWidget::changeSourceVisualState(VisualState newState){
    engine->changeSourceVisualState(newState);
}

void GLWidget::changeDestinationVisualState(VisualState newState){
    engine->changeDestinationVisualState(newState);
}

void GLWidget::handleKeyEvent(QKeyEvent *e){
    engine->handleKeyEvent(e);
}

void GLWidget::paintGL()
{
    //qDebug() << "paintGL() called";
    engine->render();
}

void GLWidget::resizeGL(int w, int h)
{
    engine->resize(w, h);
}
