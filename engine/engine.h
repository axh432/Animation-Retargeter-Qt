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

class Engine : protected QOpenGLFunctions {

public:
    Engine();
    ~Engine();
    void render();
    void resize(int w, int h);
    void updateWorld(qint64 delta);
    void handleKeyEvent(QKeyEvent *e);

private:
    void initializeGL();
    void loadMaterial(QString materialName);
    void loadModel(QString modelPath);
    void loadAnim(QString animPath);
    void updateEntities(double delta);
    void createEntities();
    unique_ptr<QOpenGLShaderProgram> createShaderProgram(QString vertexShaderPath, QString fragmentShaderPath);
    unique_ptr<QOpenGLTexture> loadTexture(QString texturePath);
    void createMaterial(DataBuffer& buffer);

private:
    unique_ptr<Entity> entity;
    unique_ptr<Entity> entity2;
    unique_ptr<Camera> camera;
    unique_ptr<ResourceManager> resourceManager;
    QMatrix4x4 projection;
};

#endif
