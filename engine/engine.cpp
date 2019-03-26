#include "engine.h"

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include <QPair>
#include <math.h>
#include <memory>
#include <iostream>

#include "lib/TextProcessor/databuffer.h"
#include "lib/TextProcessor/textprocessor.h"
#include "lib/md5/md5factory.h"
#include "lib/md5/md5.h"
#include "engine/material.h"
#include "engine/mapping.h"

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

QPair<Skeleton*, Skeleton*> Engine::getSkeletonsForRetargeting(){

    if(source && destination){
        return QPair<Skeleton*, Skeleton*>(source->getModel()->getBindPose(), destination->getModel()->getBindPose());
    }else{
        return QPair<Skeleton*, Skeleton*>(nullptr, nullptr);
    }

}

void Engine::retargetAnimation(vector<int> jointMappings){

    Skeleton* sourceBindPose = source->getModel()->getBindPose();
    Skeleton* destBindPose = destination->getModel()->getBindPose();


    for(int i = 0; i < jointMappings.size(); i++){

        int destJointIndex = jointMappings[i];
        QString destJointName = "None";

        if(destJointIndex != -1){
            destJointName = destBindPose->getJoints()[destJointIndex].name;
        }

        qDebug() << sourceBindPose->getJoints()[i].name << " : " << destJointName;

    }

    Skeleton* sourceRetargetSkel = &source->getAnim()->skeletons[10];

    destRetargetSkel.reset(new Skeleton(destBindPose));

    destRetargetSkel->recomputeLocalSpace();

    vector<JointModification> differences = Skeleton::getDifferences(sourceRetargetSkel, destRetargetSkel.get());

    destRetargetSkel->applyModificationsDifferentSkeleton(differences, jointMappings);

    source->freezeWithSkeleton(sourceRetargetSkel);

    destination->freezeWithSkeleton(destRetargetSkel.get());

}

void Engine::changeSourceAnimState(AnimState newState){
    if(source.get()){
        source->changeAnimState(newState);
    }
}

void Engine::changeDestinationAnimState(AnimState newState){
    if(destination.get()){
        destination->changeAnimState(newState);
    }
}

void Engine::changeSourceVisualState(VisualState newState){
    if(source.get()){
        source->changeVisualState(newState);
    }
}

void Engine::changeDestinationVisualState(VisualState newState){
    if(destination.get()){
        destination->changeVisualState(newState);
    }
}

void Engine::loadEntityResources(std::shared_ptr<EntityResourcePaths> sourcePaths, std::shared_ptr<EntityResourcePaths> destinationPaths){

    loadMaterial(sourcePaths->materialPath);
    loadModel(sourcePaths->modelPath, sourcePaths->modelPath);
    loadAnim(sourcePaths->animationPath, sourcePaths->animationPath);

    loadMaterial(destinationPaths->materialPath);
    loadModel(destinationPaths->modelPath,  destinationPaths->modelPath);

}

void Engine::createGraphicsCardSpaceForEntities(QString& sourceModelName, QString& destinationModelName){

    GraphicsCardSpace spaceForSource = resourceManager->computeGraphicsCardSpaceForModel(sourceModelName);
    GraphicsCardSpace spaceForDestination = resourceManager->computeGraphicsCardSpaceForModel(destinationModelName);
    GraphicsCardSpace total = spaceForSource.add(spaceForDestination);

    graphicsCardMemoryManager.reset(new GraphicsCardMemoryManager(total));
}

QMatrix4x4 Engine::createSourcePosition(){

    QMatrix4x4 matrix;
    matrix.translate(0.0f, -35.0f, -150.0f);
    matrix.rotate(-90.0f, QVector3D(1.0, 0.0, 0.0));

    return matrix;
}

QMatrix4x4 Engine::createDestinationPosition(){

    QMatrix4x4 matrix2;
    matrix2.translate(0.0f, -35.0f, -200.0f);
    matrix2.rotate(-90.0f, QVector3D(1.0, 0.0, 0.0));

    return matrix2;
}

void Engine::setEntities(std::shared_ptr<EntityResourcePaths> sourcePaths, std::shared_ptr<EntityResourcePaths> destinationPaths){

    source.reset(new Entity(
                     graphicsCardMemoryManager->createGLModel(sourcePaths->modelPath, resourceManager.get()),
                     resourceManager->getAnim(sourcePaths->animationPath),
                     createSourcePosition()));

    destination.reset(new Entity(
                          graphicsCardMemoryManager->createGLModel(destinationPaths->modelPath, resourceManager.get()),
                          createDestinationPosition()));


}

void Engine::createEntities(std::shared_ptr<EntityResourcePaths> sourcePaths, std::shared_ptr<EntityResourcePaths> destinationPaths){

    loadEntityResources(sourcePaths, destinationPaths);

    createGraphicsCardSpaceForEntities(sourcePaths->modelPath, destinationPaths->modelPath);

    setEntities(sourcePaths, destinationPaths);
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

    resourceManager->storeMaterial(buffer, materialPath);
}

void Engine::updateEntities(double delta){

    if(source.get()){
        source->update(delta);
    }

    if(destination.get()){
        destination->update(delta);
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
    source.reset(nullptr);
    destination.reset(nullptr);
    camera.reset(nullptr);
    std::cout << "Deleting engine" << std::endl;
}

void Engine::updateWorld(qint64 delta){
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

    if(source.get()){
        source->render(camera->getViewMatrix());
    }

    if(destination.get()){
        destination->render(camera->getViewMatrix());
    }

}
