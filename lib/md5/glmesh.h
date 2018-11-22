#ifndef GLMESH_H
#define GLMESH_H

#include "md5.h"
#include "engine/material.h"
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>

class GLMesh : protected QOpenGLFunctions{
public:
    GLMesh(Mesh* mesh, Material* material);
    ~GLMesh();
    void update(Skeleton* skeleton);
    void render(QMatrix4x4& mvp_matrix);
    void checkGLValidity();
    inline QOpenGLBuffer* getVertexBuffer(){ return &vertices; }
    inline QOpenGLBuffer* getIndexBuffer(){ return &mesh->getGLData().indices; }
    inline QOpenGLBuffer* getTextureCoords(){ return &mesh->getGLData().textureCoords; }

    inline Material* getMaterial(){ return material; }

private:
    int numIndices;
    QOpenGLBuffer vertices;
    Mesh* mesh;
    Material* material;

    void ifBuffersNotCreatedCreateBuffers();
};

class GLModel{
public:
    GLModel(std::vector<GLMesh> meshes, Model* model);
    void update(Skeleton* skeleton);
    void render(QMatrix4x4& mvp_matrix);
    void checkGLValidity();

    inline std::vector<GLMesh> getGLMeshes(){ return meshes; }

private:
    std::vector<GLMesh> meshes;
    Model* model;
};

#endif // GLMESH_H
