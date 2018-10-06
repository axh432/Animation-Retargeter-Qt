#ifndef GLMESH_H
#define GLMESH_H

#include "md5.h"
#include "engine/material.h"
#include <QOpenGLBuffer>

class GLMesh{
public:
    GLMesh(Mesh* mesh, Material* material);
    ~GLMesh();
    void update(Skeleton* skeleton);
    void render(QMatrix4x4& mvp_matrix);
    inline QOpenGLBuffer* getVertexBuffer(){ return &vertices; }
    inline QOpenGLBuffer* getIndexBuffer(){ return &mesh->getGLData().indices; }
    inline QOpenGLBuffer* getTextureCoords(){ return &mesh->getGLData().textureCoords; }

    inline Material* getMaterial(){ return material; }

private:
    int numIndices;
    QOpenGLBuffer vertices;
    Mesh* mesh;
    Material* material;

    void ifVertexBufferNotCreatedCreateVertexBuffer();
};

class GLModel{
public:
    GLModel(std::vector<GLMesh> meshes, Model* model);
    void update(Skeleton* skeleton);
    void render(QMatrix4x4& mvp_matrix);

    inline std::vector<GLMesh> getGLMeshes(){ return meshes; }

private:
    std::vector<GLMesh> meshes;
    Model* model;
};

#endif // GLMESH_H
