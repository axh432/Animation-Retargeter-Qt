#ifndef ENGINE_H
#define ENGINE_H

#include "engine/geometryengine.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QKeyEvent>

#include "camera.h"
#include "entity.h"
#include "material.h"
#include "lib/TextProcessor/databuffer.h"
#include "resourcemanager.h"
#include "graphicscardmemorymanager.h"
#include "entityresourcepaths.h"
#include "ui/commands.h"


class Engine : protected QOpenGLFunctions {

public:
    Engine();
    ~Engine();
    void render();
    void resize(int w, int h);
    void updateWorld(qint64 delta);
    void handleKeyEvent(QKeyEvent *e);
    void createEntities(std::shared_ptr<EntityResourcePaths> sourcePaths, std::shared_ptr<EntityResourcePaths> destinationPaths);
    void changeSourceAnimState(AnimState newState);
    void changeDestinationAnimState(AnimState newState);
    void changeSourceVisualState(VisualState newState);
    void changeDestinationVisualState(VisualState newState);
    QPair<Skeleton*, Skeleton*> getSkeletonsForRetargeting();
    void retargetAnimation(vector<int> jointMappings);

private:
    void initializeGL();
    void loadMaterial(QString materialName);
    void loadModel(QString modelPath, QString name);
    void loadAnim(QString animPath, QString name);
    void loadResource(QString resourcePath, QString schemaPath, DataBuffer* buffer);
    void updateEntities(double delta);
    unique_ptr<QOpenGLShaderProgram> createShaderProgram(QString vertexShaderPath, QString fragmentShaderPath);
    unique_ptr<QOpenGLTexture> loadTexture(QString texturePath);
    void createMaterial(DataBuffer& buffer);
    void createStringMapping();
    void printMappings(Skeleton* from, Skeleton* to);
    vector<int> createMappings(Skeleton* from);
    QString findClosestToName(QString name, vector<QString>& potentialMatches);
    void loadEntityResources(std::shared_ptr<EntityResourcePaths> sourcePaths, std::shared_ptr<EntityResourcePaths> destinationPaths);
    void createGraphicsCardSpaceForEntities(QString& sourceModelName, QString& destinationModelName);
    void setEntities(std::shared_ptr<EntityResourcePaths> sourcePaths, std::shared_ptr<EntityResourcePaths> destinationPaths);
    QMatrix4x4 createDestinationPosition();
    QMatrix4x4 createSourcePosition();

private:
    unique_ptr<Entity> source;
    unique_ptr<Entity> destination;
    unique_ptr<Skeleton> copy;
    unique_ptr<Camera> camera;
    unique_ptr<ResourceManager> resourceManager;
    unique_ptr<GraphicsCardMemoryManager> graphicsCardMemoryManager;
};

#endif
