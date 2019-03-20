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

QString Engine::findClosestToName(QString name, vector<QString>& potentialMatches){

    for(int i = 0; i < potentialMatches.size(); i++){

        if(name.compare(potentialMatches[i], Qt::CaseInsensitive) == 0){
            return potentialMatches[i];
        }

    }

    return "";

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

}

void Engine::printMappings(Skeleton* from, Skeleton* to){

    vector<QString> fromNames;

    vector<Joint>& fromJoints = from->getJoints();
    vector<Joint>& toJoints = to->getJoints();

    for(int i = 0; i < fromJoints.size(); i++){
        fromNames.push_back(fromJoints[i].name);
    }

    for(int i = 0; i < toJoints.size(); i++){

        QString toName = toJoints[i].name;

        QString greatestMatch = findClosestToName(toName, fromNames);

        qDebug() << "jointNameMapping.push_back({ " << toName << ", " << greatestMatch << " });";
    }

}

vector<int> Engine::createMappings(Skeleton* from){

    vector<StringMapping> jointNameMapping;
    vector<int> indexMapping;

    //toJoint, fromJoint
    jointNameMapping.push_back({  "origin" ,  "origin"  });
    jointNameMapping.push_back({  "Body" ,  "Body"  });
    jointNameMapping.push_back({  "Hips" ,  "Hips"  });
    jointNameMapping.push_back({  "Lupleg" ,  "Lupleg"  });
    jointNameMapping.push_back({  "Lloleg" ,  "Lloleg"  });
    jointNameMapping.push_back({  "Lankle_r" ,  "Lankle_r"  });
    jointNameMapping.push_back({  "Lball_r" ,  "Lball_r"  });
    jointNameMapping.push_back({  "Ltoe1_r" ,  ""  });
    jointNameMapping.push_back({  "Ltip1_r" ,  ""  });
    jointNameMapping.push_back({  "Ltoe2_r" ,  ""  });
    jointNameMapping.push_back({  "Lknee" ,  "Lknee"  });
    jointNameMapping.push_back({  "Rupleg" ,  "Rupleg"  });
    jointNameMapping.push_back({  "Rloleg" ,  "Rloleg"  });
    jointNameMapping.push_back({  "Rankle_r" ,  "Rankle_r"  });
    jointNameMapping.push_back({  "Rball_r" ,  "Rball_r"  });
    jointNameMapping.push_back({  "Rtoe1_r" ,  ""  });
    jointNameMapping.push_back({  "Rtip1_r" ,  ""  });
    jointNameMapping.push_back({  "Rtoe2_r" ,  ""  });
    jointNameMapping.push_back({  "Rtip2_r" ,  ""  });
    jointNameMapping.push_back({  "Rknee" ,  "Rknee"  });
    jointNameMapping.push_back({  "Waist" ,  "waist"  });
    jointNameMapping.push_back({  "Chest" ,  "chest"  });
    jointNameMapping.push_back({  "Lrib" ,  ""  });
    jointNameMapping.push_back({  "Rrib" ,  ""  });
    jointNameMapping.push_back({  "Shoulders" ,  "shoulders"  });
    jointNameMapping.push_back({  "Lshldr" ,  "Lshldr"  });
    jointNameMapping.push_back({  "Luparm" ,  "Luparm"  });
    jointNameMapping.push_back({  "Lloarm" ,  "Lloarm"  });
    jointNameMapping.push_back({  "Lelbow" ,  ""  });
    jointNameMapping.push_back({  "Lhand" ,  "Lhand"  });
    jointNameMapping.push_back({  "Lindex_lo" ,  ""  });
    jointNameMapping.push_back({  "Lindex_base" ,  ""  });
    jointNameMapping.push_back({  "Lindex_mid" ,  ""  });
    jointNameMapping.push_back({  "Lindex_tip" ,  ""  });
    jointNameMapping.push_back({  "Lmissile" ,  ""  });
    jointNameMapping.push_back({  "Lpinky_base" ,  ""  });
    jointNameMapping.push_back({  "Lpinky_mid" ,  ""  });
    jointNameMapping.push_back({  "Lpinky_tip" ,  ""  });
    jointNameMapping.push_back({  "Lring_lo" ,  ""  });
    jointNameMapping.push_back({  "Lring_base" ,  ""  });
    jointNameMapping.push_back({  "Lring_mid" ,  ""  });
    jointNameMapping.push_back({  "Lring_tip" ,  ""  });
    jointNameMapping.push_back({  "Lthumb_lo" ,  ""  });
    jointNameMapping.push_back({  "Lthumb_base" ,  ""  });
    jointNameMapping.push_back({  "Lthumb_mid" ,  ""  });
    jointNameMapping.push_back({  "Lthumb_tip" ,  ""  });
    jointNameMapping.push_back({  "Neck" ,  ""  });
    jointNameMapping.push_back({  "Head" ,  "head"  });
    jointNameMapping.push_back({  "Jaw" ,  ""  });
    jointNameMapping.push_back({  "camera" ,  ""  });
    jointNameMapping.push_back({  "Rshldr" ,  "Rshldr"  });
    jointNameMapping.push_back({  "Ruparm" ,  "Ruparm"  });
    jointNameMapping.push_back({  "Rloarm" ,  "Rloarm"  });
    jointNameMapping.push_back({  "Relbow" ,  ""  });
    jointNameMapping.push_back({  "Rhand" ,  "Rhand"  });
    jointNameMapping.push_back({  "Rindex_lo" ,  ""  });
    jointNameMapping.push_back({  "Rindex_base" ,  ""  });
    jointNameMapping.push_back({  "Rindex_mid" ,  ""  });
    jointNameMapping.push_back({  "Rindex_tip" ,  ""  });
    jointNameMapping.push_back({  "Rmissile" ,  ""  });
    jointNameMapping.push_back({  "Rpinky_base" ,  ""  });
    jointNameMapping.push_back({  "Rpinky_mid" ,  ""  });
    jointNameMapping.push_back({  "Rpinky_tip" ,  ""  });
    jointNameMapping.push_back({  "Rring_lo" ,  ""  });
    jointNameMapping.push_back({  "Rring_base" ,  ""  });
    jointNameMapping.push_back({  "Rring_mid" ,  ""  });
    jointNameMapping.push_back({  "Rring_tip" ,  ""  });
    jointNameMapping.push_back({  "Rthumb_lo" ,  ""  });
    jointNameMapping.push_back({  "Rthumb_base" ,  ""  });
    jointNameMapping.push_back({  "Rthumb_mid" ,  ""  });
    jointNameMapping.push_back({  "Rthumb_tip" ,  ""  });

    for(int i = 0; i < jointNameMapping.size(); i++){

        QString& fromJointName = jointNameMapping[i].fromJointName;

        if(fromJointName.isEmpty()){
            indexMapping.push_back(-1);
        }else{
            indexMapping.push_back(from->getJointIndex(fromJointName));
            qDebug() << fromJointName << " : " << from->getJointIndex(fromJointName);
        }

    }

    return indexMapping;
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

    //qDebug() << "qint64 delta: " << delta;

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
