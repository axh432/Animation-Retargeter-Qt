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

class GeometryEngine;

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
    void initShaders();
    void initTextures();
    void initMaterials();
    void testSimpleModel();
    unique_ptr<Model> loadModel();
    unique_ptr<Anim> loadAnim();
    void updateEntity(double delta);
    void createEntity();
    unique_ptr<QOpenGLShaderProgram> createShaderProgram(QString vertexShaderPath, QString fragmentShaderPath);
    unique_ptr<QOpenGLTexture> loadTexture(QString texturePath);
    void createMaterial(DataBuffer& buffer);

private:
    unique_ptr<Entity> entity;

    QOpenGLShaderProgram program;
    GeometryEngine *geometries;
    Camera *camera;

    QOpenGLTexture *texture;
    QOpenGLTexture *imp;

    QHash<QString, Material*> materials;
    std::vector<Material> materialStorage;

    QMatrix4x4 projection;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed;
    QQuaternion rotation;
};

#endif