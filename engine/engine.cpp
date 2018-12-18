#include "engine.h"

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include <math.h>
#include <memory>
#include <iostream>

#include "lib/TextProcessor/databuffer.h"
#include "lib/TextProcessor/textprocessor.h"
#include "lib/md5/md5factory.h"
#include "lib/md5/md5.h"
#include "engine/material.h"

using std::unique_ptr;

Engine::Engine() :
    camera(new Camera(QVector3D(0,0,0)))
{
    initializeGL();
    resourceManager.reset(new ResourceManager());
}

void Engine::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

}

void Engine::createEntities(){
    loadMaterial(":/imp.mtr");
    loadModel(":/imp.md5mesh", "imp");
    loadAnim(":/evade_left.md5anim", "evade_left");

    loadMaterial(":/fatty.mtr");
    loadModel(":/fatty.md5mesh", "fatty");
    loadAnim(":/walk1.md5anim", "walk1");

    QMatrix4x4 matrix;
    matrix.translate(0.0f, -35.0f, -150.0f);
    matrix.rotate(-90.0f, QVector3D(1.0, 0.0, 0.0));

    QMatrix4x4 matrix2;
    matrix2.translate(0.0f, -35.0f, -300.0f);
    matrix2.rotate(-90.0f, QVector3D(1.0, 0.0, 0.0));

    GraphicsCardSpace spaceForFatty = resourceManager->computeGraphicsCardSpaceForModel("fatty");
    GraphicsCardSpace spaceForImp = resourceManager->computeGraphicsCardSpaceForModel("imp");

    GraphicsCardSpace total = spaceForFatty.add(spaceForImp);

    graphicsCardMemoryManager.reset(new GraphicsCardMemoryManager(total));

    entity.reset(new Entity(graphicsCardMemoryManager->createGLModel("fatty", resourceManager.get()), resourceManager->getAnim("walk1"), matrix));
    entity2.reset(new Entity(graphicsCardMemoryManager->createGLModel("imp", resourceManager.get()), resourceManager->getAnim("evade_left"), matrix2));
}

void Engine::loadResource(QString resourcePath, QString schemaPath, DataBuffer* buffer){

    QFile infile(resourcePath);
    QFile schemaFile(schemaPath);

    if(!infile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qCritical() << "Unable to open file: " << infile.fileName();
    }
    qDebug() << "Opened file: " << infile.fileName();

    if(!schemaFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qCritical() << "Unable to open file: " << schemaFile.fileName();
    }
    qDebug() << "Opened file: " << schemaFile.fileName();

    unique_ptr<QTextStream> in(new QTextStream(&infile));
    unique_ptr<QTextStream> schema(new QTextStream(&schemaFile));

    unique_ptr<TextProcessor> processor(new TextProcessor(std::move(in), std::move(schema), buffer));
    qDebug("Created Processor");

    processor->parse();
    qDebug("finished parsing");

    qDebug() << "Parse Result: \n\tInt Buffer size - " << buffer->getIntBufferSize()
             << " \n\tFloat Buffer size - " << buffer->getFloatBufferSize()
             << " \n\tString Buffer size - " << buffer->getStringBufferSize();

}

void Engine::loadModel(QString modelPath, QString name){
    DataBuffer buffer;

    loadResource(modelPath, ":/md5mesh.schema", &buffer);

    resourceManager->storeModel(&buffer, name);
}

void Engine::loadAnim(QString animPath, QString name){
    DataBuffer buffer;

    loadResource(animPath, ":/md5anim.schema", &buffer);

    resourceManager->storeAnim(&buffer, name);
}

void Engine::loadMaterial(QString materialPath){
    DataBuffer buffer;

    loadResource(materialPath, ":/mtr.schema", &buffer);

    resourceManager->storeMaterial(buffer);
}

void Engine::updateEntities(double delta){

    if(entity.get()){
        entity->update(delta);
    }

    if(entity2.get()){
        entity2->update(delta);
    }

}

void Engine::handleKeyEvent(QKeyEvent *e){

    switch(e->key()){
        case Qt::Key_W:
            camera->moveForward();
            break;

        case Qt::Key_S:
            camera->moveBackward();
            break;

        case Qt::Key_A:
            camera->moveLeft();
            break;

        case Qt::Key_D:
            camera->moveRight();
            break;

        case Qt::Key_R:
            camera->moveUp();
            break;

        case Qt::Key_F:
            camera->moveDown();
            break;

        case Qt::Key_Up:
            camera->lookUp();
            break;

        case Qt::Key_Down:
            camera->lookDown();
            break;

        case Qt::Key_Left:
            camera->lookLeft();
            break;

        case Qt::Key_Right:
            camera->lookRight();
            break;
    }


}

Engine::~Engine()
{
    entity.reset(nullptr);
    entity2.reset(nullptr);
    camera.reset(nullptr);
    std::cout << "Deleting engine" << std::endl;
}

void Engine::updateWorld(qint64 delta){

    qDebug() << "qint64 delta: " << delta;

    double deltaAsDouble = (double) delta;

    updateEntities(deltaAsDouble);
}


void Engine::resize(int w, int h)
{
    camera->resize(w, h);
}

void Engine::render()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(entity.get()){
        //entity->getModel()->checkGLValidity();
        entity->render(camera->getViewMatrix());
    }

    if(entity2.get()){
        entity2->render(camera->getViewMatrix());
    }

}
